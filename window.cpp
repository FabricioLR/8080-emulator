#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

#include "8080.h"

const int WIDTH = 224;
const int HEIGHT = 256;
const int RECTANGLE_SIZE = 2;

int main(){
    FILE *file;
    int char1;
    int i = 0;

    file = fopen("rom/invaders.rom", "rb");

    while (1){
        char1 = fgetc(file);
        if (char1 == EOF){
            break;
        }

        memory[i] = char1;
        i++;
    }

    fclose(file);

	SDL_Window* window = NULL;

	if(SDL_Init( SDL_INIT_VIDEO ) < 0){
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH * RECTANGLE_SIZE, HEIGHT * RECTANGLE_SIZE, SDL_WINDOW_SHOWN);
	if(window == NULL){
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return 0;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        return 0;
    }

    double now;
    double lastTimer;
    double nextInterrupt;
    double sinceLastCycle;

    SDL_Event event;
    int pending = 0;
    while(1){
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

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        int index = 0;
		for (int ix = 0; ix < WIDTH; ix++){
			for (int iy = 0; iy < HEIGHT; iy += 8){
				uint8_t byte = memory[0x2400 + index];

				for (int b = 0; b < 8; b++) {
					SDL_Rect squareRect;
					squareRect.w = RECTANGLE_SIZE;
					squareRect.h = RECTANGLE_SIZE;
                    squareRect.x = ix * RECTANGLE_SIZE;
					squareRect.y = (HEIGHT - iy - b) * RECTANGLE_SIZE;

					if ((byte & 0x1) == 0){
						SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
					} else {
						SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
					}

					SDL_RenderFillRect(renderer, &squareRect);
					byte >>= 1;
				}
                index++;
			}
		}

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
