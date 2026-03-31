/*
 * syscalls.c
 *
 *  Created on: Mar 29, 2026
 *      Author: Salih
 */

#include <stdint.h>


void int_to_str(uint16_t num, char* str) {
    int i = 0;
    if (num == 0) {
        str[i++] = '0';
    } else {
        char buf[5];
        int j = 0;
        while (num > 0) {
            buf[j++] = (num % 10) + '0';
            num /= 10;
        }
        while (j > 0) {
            str[i++] = buf[--j];
        }
    }
    str[i] = '\0';
}
