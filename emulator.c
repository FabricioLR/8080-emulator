#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "8080.h"

int main() { 
    FILE *file;
    int char1;

    file = fopen("rom/invaders.rom", "rb");

    int i = 0;
    while (1){
        char1 = fgetc(file);

        if (char1 == EOF){
            break;
        }

        memory[i] = char1;
        i++;
    }

    fclose(file);

    double now;
    double lastTimer;
    double nextInterrupt;
    double sinceLastCycle;

    while (1) {
        struct timeval currentTime;
        gettimeofday(&currentTime, NULL);
        now = ((double)currentTime.tv_sec * 1E6) + ((double)currentTime.tv_usec);

        if (lastTimer == 0.0){
            lastTimer = now;
            nextInterrupt = lastTimer + 16000.0;
            whichInterrupt = 1;
        }

        if ((int_enable) && (now > nextInterrupt)){
            if (whichInterrupt == 1){
                
                memory[sp - 2] = pc & 0xff;
                memory[sp - 1] = (pc & 0xff00) >> 8;
                sp -= 2;
                
                pc = 8 * 1;
                int_enable = 0;
                whichInterrupt = 2;
            } else {
                memory[sp - 2] = pc & 0xff;
                memory[sp - 1] = (pc & 0xff00) >> 8;
                sp -= 2;
                
                pc = 8 * 2;
                int_enable = 0;
                whichInterrupt = 1;
            }
            nextInterrupt = now + 8000.0;
        }

        double sinceLast = now - lastTimer;
        int cycles_to_catch_up = 2 * sinceLast;
        int cycles1 = 0;

        while (cycles_to_catch_up > cycles1){
            cycles1 += emulate();
        }
        lastTimer = now;
    }

    return 0;
}
