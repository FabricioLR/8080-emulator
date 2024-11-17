#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

#include "8080.h"

#define FRAME_ROWS 256
#define FRAME_COLS 224

#define ROWS FRAME_ROWS
#define COLS FRAME_COLS

#define BLACK_R 0
#define BLACK_G 0
#define BLACK_B 0

#define WHITE_R 255
#define WHITE_G 255
#define WHITE_B 255

#define ALPHA 255

int main(){
    FILE *file;
    int char1;
    int i = 0;

    file = fopen("invaders.rom", "rb");

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
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    uint8_t *framebuf;
    int pending = 0;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(COLS, ROWS, 0, &window, &renderer);
    while (1) {
        pending = SDL_PollEvent(&event);
        if (pending && event.type == SDL_QUIT) {
            break;
        }

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

        SDL_SetRenderDrawColor(renderer, BLACK_R, BLACK_G, BLACK_B, ALPHA);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, WHITE_R, WHITE_G, WHITE_B, ALPHA);

        uint8_t pixels_octet, mask, value;

        for (int j = 0; j < COLS; j++) {
            for (int i = 0; i < ROWS;) {
                int offset = (j * ROWS + i) / 8;
                printf("%d - ", offset);
                pixels_octet = memory[0x2400 + offset];

                for (uint8_t b = 0; b < 8; b++) {
                    mask = 1 << b;
                    value = mask & pixels_octet;
                    if (value) {
                        SDL_RenderDrawPoint(renderer, j, ROWS - i);
                    }
                    i++;
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
