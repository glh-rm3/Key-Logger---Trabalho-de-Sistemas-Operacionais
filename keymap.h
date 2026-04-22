#include <stdbool.h>
#include <linux/input.h>

#define KEYMAP_H
#define KEY_MAX_CODE 0x2ff

const char *key_code_names[KEY_MAX_CODE + 1]= {
    [KEY_A] = "A",
    [KEY_B] = "B",
    [KEY_C] = "C",
    [KEY_D] = "D",
    [KEY_E] = "E",
    [KEY_F] = "F",
    [KEY_G] = "G",
    [KEY_H] = "H",
    [KEY_I] = "I",
    [KEY_J] = "J",
    [KEY_K] = "K",
    [KEY_L] = "L",
    [KEY_M] = "M",
    [KEY_N] = "N",
    [KEY_O] = "O",
    [KEY_P] = "P",
    [KEY_Q] = "Q",
    [KEY_R] = "R",
    [KEY_S] = "S",
    [KEY_T] = "T",
    [KEY_U] = "U",
    [KEY_V] = "V",
    [KEY_W] = "W",
    [KEY_X] = "X",
    [KEY_Y] = "Y",
    [KEY_Z] = "Z",


    [KEY_ENTER] = "\n",
    [KEY_SPACE] = " ",
    [KEY_BACKSPACE] = "\b",
    [KEY_LEFTSHIFT] = " _lsf_ ",
    [KEY_RIGHTSHIFT] = " _rsf_ ",
    [KEY_LEFTCTRL] = " _ctrleft_ ",
    [KEY_RIGHTCTRL] = " _ctrlright_ ",
    [KEY_LEFTALT] = " _leftalt_ ",
    [KEY_RIGHTALT] = " _rightalt_ ",
    [KEY_TAB] = " _tab_ ",
    [KEY_CAPSLOCK] = " _caps_ ",
    [KEY_UP] = " _up_ ",
    [KEY_DOWN] = " _down_ ",
    [KEY_LEFT] = " _left_ ",
};
