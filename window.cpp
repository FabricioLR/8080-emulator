#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

const int WIDTH = 224;
const int HEIGHT = 256;
const int RECTANGLE_SIZE = 2;

const uint16_t videomemory = 0x2400;

unsigned char memory[0x10000];

__uint16_t pc = 0;
__uint16_t sp = 0;
__uint8_t a = 0;
__uint8_t b = 0;
__uint8_t c = 0;
__uint8_t d = 0;
__uint8_t e = 0;
__uint8_t h = 0;
__uint8_t l = 0;

__uint8_t CF = 0;
__uint8_t PF = 0;
__uint8_t AF = 0;
__uint8_t ZF = 0;
__uint8_t SF = 0;

int int_enable = 0;
int cycles = 0;

int parity(int x, int size){
	int parity = 0;

	int i;
	for (i = 0; i < size; i++){
		parity += x & 1;
		x = x >> 1;
	}

	return (parity % 2 == 0);
}


int emulate(){
	switch (memory[pc]){
        case 0x00:
            //printf("%02x   NOOP\n", pc);
            cycles = 4;
            pc++;
            break;
        case 0x01:
            //printf("%02x   LXI B, %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            b = memory[pc + 2];
            c = memory[pc + 1];
            pc += 3;
            cycles = 10;
            break;
        case 0x02:
            //printf("%02x   STAX B\n", pc);
            memory[(b << 8) | c] = a;
            pc++;
            cycles = 7;
            break;
        case 0x03:
            //printf("%02x   INX B\n", pc);
            c++;
            if (c == 0) b++;
            pc++;
            cycles = 5;
            break;
        case 0x04:
            //printf("%02x   INR B\n", pc);
            {
                __uint16_t result = (__uint16_t)b + 1;
                b = result & 0x00FF;

                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80)); //res & 1000 0000
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09); //checks if result is greater than 9
            }
            pc++;
            cycles = 5;
            break;
        case 0x05:
            //printf("%02x   DCR B\n", pc);
            {
                __uint16_t result = (__uint16_t)b - 1;
                b = result & 0xFF;

                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
            }
            pc++;
            cycles = 5;
            break;
        case 0x06:
            //printf("%02x   MVI B, %02x\n", pc, memory[pc + 1]);
            b = memory[pc + 1];
            pc += 2;
            cycles = 7;
            break;
        case 0x07:
            //printf("%02x   RLC\n", pc);
            {
                __uint8_t temp = a;
                a = temp << 1 | temp >> 7;
                CF = ((temp >> 7) > 0);
            }
            pc++;
            cycles = 4;
            break;
        case 0x08:
            //printf("%02x   NOOP\n", pc);
            pc++;
            cycles = 4;
            break;
        case 0x09:
            //printf("%02x   DAD B\n", pc);
            {
                __uint32_t hl = (h << 8) | l;
                __uint32_t bc = (b << 8) | c;
                __uint32_t result = (hl + bc);

                CF = ((result & 0xffff0000) > 0);

                h = (result & 0xff00) >> 8;
                l = (result & 0xff);
            }
            pc++;
            cycles = 10;
            break;
        case 0x0A:
            //printf("%02x   LDAX B\n", pc);
            a = memory[(b << 8) | c];
            pc++;
            cycles = 7;
            break;
        case 0x0B:
            //printf("%02x   DCX B\n", pc);
            c--;
            if (c == 0xff) b--;
            pc++;
            cycles = 5;
            break;
        case 0x0C:
            //printf("%02x   INR C\n", pc);
            {
                __uint16_t result = (__uint16_t)c + 1;
                c = result & 0x00FF;

                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80)); //res & 1000 0000
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09); //checks if result is greater than 9
            }
            pc++;
            cycles = 5;
            break;
        case 0x0D:
            //printf("%02x   DCR C\n", pc);
            {
                __uint16_t result = (__uint16_t)c - 1;
                c = result & 0xFF;

                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
            }
            pc++;
            cycles = 5;
            break;
        case 0x0E:
            //printf("%02x   MVI C, %02x\n", pc, memory[pc + 1]);
            c = memory[pc + 1];
            pc += 2;
            cycles = 7;
            break;
        case 0x0F:
            //printf("%02x   RRC\n", pc);
            {
                __uint8_t temp = a;
                a = temp >> 1 | temp << 7;

                CF = ((a >> 7) > 0);
            }
            pc++;
            cycles = 4;
            break;
        case 0x10:
            //printf("%02x   NOOP\n", pc);
            pc++;
            cycles = 4;
            break;
        case 0x11:
            //printf("%02x   LXI D, %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            d = memory[pc + 2];
            e = memory[pc + 1];
            pc += 3;
            cycles = 10;
            break;
        case 0x12:
            //printf("%02x   STAX D\n", pc);
            memory[(d << 8) | e] = a;
            pc++;
            cycles = 7;
            break;
        case 0x13:
            //printf("%02x   INX D\n", pc);
            e++;
            if (e == 0) d++;
            pc++;
            cycles = 5;
            break;
        case 0x14:
            //printf("%02x   INR D\n", pc);
            {
                __uint16_t result = (__uint16_t)d + 1;
                d = result & 0x00FF;

                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80)); //res & 1000 0000
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09); //checks if result is greater than 9
            }
            pc++;
            cycles = 5;
            break;
        case 0x15:
            //printf("%02x   DCR D\n", pc);
            {
                __uint16_t result = (__uint16_t)d - 1;
                d = result & 0xFF;

                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
            }
            pc++;
            cycles = 5;
            break;
        case 0x16:
            //printf("%02x   MVI D, %02x\n", pc, memory[pc + 1]);
            d = memory[pc + 1];
            pc += 2;
            cycles = 7;
            break;
        case 0x17:
            //printf("%02x   RAL\n", pc);
            {
                __uint8_t temp = a;
                __uint8_t msb = (a >> 7);
                a = (temp << 1) | (CF);
                CF = msb;
            }
            pc++;
            cycles = 4;
            break;
        case 0x18:
            //printf("%02x   NOOP\n", pc);
            pc++;
            cycles = 4;
            break;
        case 0x19:
            //printf("%02x   DAD D\n", pc);
            {
                __uint16_t hl = (h << 8) | l;
                __uint16_t de = (d << 8) | e;
                __uint32_t result = (hl + de);

                CF = ((result & 0xffff0000) > 0);

                h = (result & 0xff00) >> 8;
                l = (result & 0xff);
            }
            pc++;
            cycles = 10;
            break;
        case 0x1A:
            //printf("%02x   LDAX D\n", pc);
            a = memory[(d << 8) | e];
            pc++;
            cycles = 7;
            break;
        case 0x1B:
            //printf("%02x   DCX D\n", pc);
            e--;
            if (e == 0xff) d--;
            pc++;
            cycles = 5;
            break;
        case 0x1C:
            //printf("%02x   INR E\n", pc);
            {
                __uint16_t result = (__uint16_t)e + 1;
                e = result & 0x00FF;

                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80)); //res & 1000 0000
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09); //checks if result is greater than 9
            }
            pc++;
            cycles = 5;
            break;
        case 0x1D:
            //printf("%02x   DCR E\n", pc);
            {
                __uint16_t result = (__uint16_t)e - 1;
                e = result & 0xFF;

                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
            }
            pc++;
            cycles = 5;
            break;
        case 0x1E:
            //printf("%02x   MVI E, %02x\n", pc, memory[pc + 1]);
            e = memory[pc + 1];
            pc += 2;
            cycles = 7;
            break;
        case 0x1F:
            //printf("%02x   RAR\n", pc);
            {
                __uint8_t temp = a;
                __uint8_t msb = ((a >> 7) << 7);
                a = (temp >> 1) | (msb);
                CF = (temp << 7) >> 7;
            }
            pc++;
            cycles = 4;
            break;

        case 0x20:
            //printf("%02x   NOOP\n", pc);
            pc++;
            cycles = 4;
            break;
        case 0x21:
            //printf("%02x   LXI H, %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            h = memory[pc + 2];
            l = memory[pc + 1];
            pc += 3;
            cycles = 10;
            break;
        case 0x22:
            //printf("%02x   SHLD %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            memory[(memory[pc + 2] << 8) | memory[pc + 1]] = l;
            memory[((memory[pc + 2] << 8) | memory[pc + 1]) + 1] = h;
            pc += 3;
            cycles = 16;
            break;
        case 0x23:
            //printf("%02x   INX H\n", pc);
            l++;
            if (l == 0) h++;
            pc++;
            cycles = 5;
            break;
        case 0x24:
            //printf("%02x   INR H\n", pc);
            {
                __uint16_t result = (__uint16_t)h + 1;
                h = result & 0x00FF;

                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80)); //res & 1000 0000
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09); //checks if result is greater than 9
            }
            pc++;
            cycles = 5;
            break;
        case 0x25:
            //printf("%02x   DCR H\n", pc);
            {
                __uint16_t result = (__uint16_t)h - 1;
                h = result & 0xFF;

                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
            }
            pc++;
            cycles = 5;
            break;
        case 0x26:
            //printf("%02x   MVI H, %02x\n", pc, memory[pc + 1]);
            h = memory[pc + 1];
            pc += 2;
            cycles = 7;
            break;
        case 0x27:
            //printf("%02x   DAA\n", pc);
            {
                __uint8_t fourLSB = ((a << 4) >> 4);
                if ((fourLSB > 0x09) || (AF == 1)){
                    __uint16_t resultLSB = fourLSB + 0x06;
                    a = a + 0x06;

                    CF = (resultLSB > 0xff);
                    ZF = ((resultLSB & 0xff) == 0);
                    SF = (0x80 == (resultLSB & 0x80));
                    PF = parity(resultLSB & 0xff, 8);
                    AF = (resultLSB > 0x09);
                }

                __uint8_t fourMSB = (a >> 4);
                if ((fourMSB > 0x09) || (CF == 1)){
                    __uint16_t resultMSB = fourMSB + 0x06;
                    a = a + 0x06;
                    
                    CF = (resultMSB > 0xff);
                    ZF = ((resultMSB & 0xff) == 0);
                    SF = (0x80 == (resultMSB & 0x80));
                    PF = parity(resultMSB & 0xff, 8);
                    AF = (resultMSB > 0x09);
                }
            }
            pc++;
            cycles = 4;
            break;
        case 0x28:
            //printf("%02x   NOOP\n", pc);
            pc++;
            cycles = 4;
            break;
        case 0x29:
            //printf("%02x   DAD H\n", pc);
            {
                __uint16_t hl = (h << 8) | l;
                __uint32_t result = (hl + hl);

                CF = ((result & 0xffff0000) > 0);

                h = (result & 0xff00) >> 8;
                l = (result & 0xff);
            }
            pc++;
            cycles = 10;
            break;
        case 0x2A:
            //printf("%02x   LHLD %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            l = memory[(memory[pc + 2] << 8) | memory[pc + 1]];
            h = memory[((memory[pc + 2] << 8) | memory[pc + 1]) + 1];
            pc += 3;
            cycles = 16;
            break;
        case 0x2B:
            //printf("%02x   DCX H\n", pc);
            l--;
            if (l == 0xff) h--;
            pc++;
            cycles = 5;
            break;
        case 0x2C:
            //printf("%02x   INR L\n", pc);
            {
                __uint16_t result = (__uint16_t)l + 1;
                l = result & 0x00FF;

                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80)); //res & 1000 0000
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09); //checks if result is greater than 9
            }
            pc++;
            cycles = 5;
            break;
        case 0x2D:
            //printf("%02x   DCR L\n", pc);
            {
                __uint16_t result = (__uint16_t)l - 1;
                l = result & 0xFF;

                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
            }
            pc++;
            cycles = 5;
            break;
        case 0x2E:
            //printf("%02x   MVI L, %02x\n", pc, memory[pc + 1]);
            l = memory[pc + 1];
            pc += 2;
            cycles = 7;
            break;
        case 0x2F:
            //printf("%02x   CMA\n", pc);
            a = ~(a);
            pc++;
            cycles = 4;
            break;

        case 0x30:
            //printf("%02x   NOOP\n", pc);
            pc++;
            cycles = 4;
            break;
        case 0x31:
            //printf("%02x   LXI SP, %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            sp = (memory[pc + 2] << 8) | (memory[pc + 1]);
            pc += 3;
            cycles = 10;
            break;
        case 0x32:
            //printf("%02x   STA %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            memory[(memory[pc + 2] << 8) | memory[pc + 1]] = a;
            pc += 3;
            cycles = 13;
            break;
        case 0x33:
            //printf("%02x   INX SP\n", pc);
            sp++;
            pc++;
            cycles = 5;
            break;
        case 0x34:
            //printf("%02x   INR M\n", pc);
            {
                __uint16_t result = (__uint16_t)memory[(h << 8) | l] + 1;
                memory[(h << 8) | l] = result & 0x00FF;

                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 10;
            break;
        case 0x35:
            //printf("%02x   DCR M\n", pc);
            {
                __uint16_t result = (__uint16_t)memory[(h << 8) | l] - 1;
                memory[(h << 8) | l] = result & 0xFF;

                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
            }
            pc++;
            cycles = 10;
            break;
        case 0x36:
            //printf("%02x   MVI M, %02x\n", pc, memory[pc + 1]);
            memory[(h << 8) | l] = memory[pc + 1];
            pc += 2;
            cycles = 10;
            break;
        case 0x37:
            //printf("%02x   STC\n", pc);
            CF = 1;
            pc++;
            cycles = 4;
            break;
        case 0x38:
            //printf("%02x   NOOP\n", pc);
            pc++;
            cycles = 4;
            break;
        case 0x39:
            //printf("%02x   DAD SP\n", pc);
            {
                __uint16_t hl = (h << 8) | l;
                __uint32_t result = (hl + sp);

                CF = ((result & 0xffff0000) > 0);

                h = (result & 0xff00) >> 8;
                l = (result & 0xff);
            }
            pc++;
            cycles = 10;
            break;
        case 0x3A:
            //printf("%02x   LDA %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            a = memory[(memory[pc + 2] << 8) | memory[pc + 1]];
            pc += 3;
            cycles = 13;
            break;
        case 0x3B:
            //printf("%02x   DCX SP\n", pc);
            sp--;
            pc++;
            cycles = 5;
            break;
        case 0x3C:
            //printf("%02x   INR A\n", pc);
            {
                __uint16_t result = (__uint16_t)a + 1;
                a = result & 0x00FF;

                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80)); //res & 1000 0000
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09); //checks if result is greater than 9
            }
            pc++;
            cycles = 5;
            break;
        case 0x3D:
            //printf("%02x   DCR A\n", pc);
            {
                __uint16_t result = (__uint16_t)a - 1;
                a = result & 0xFF;

                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
            }
            pc++;
            cycles = 5;
            break;
        case 0x3E:
            //printf("%02x   MVI A, %02x\n", pc, memory[pc + 1]);
            a = memory[pc + 1];
            pc += 2;
            cycles = 7;
            break;
        case 0x3F:
            //printf("%02x   CMC\n", pc);
            CF = ~(CF);
            pc++;
            cycles = 4;
            break;
        
        case 0x40:
            //printf("%02x   MOV B, B\n", pc);
            b = b;
            pc++;
            cycles = 5;
            break;
        case 0x41:
            //printf("%02x   MOV B, C\n", pc);
            b = c;
            pc++;
            cycles = 5;
            break;
        case 0x42:
            //printf("%02x   MOV B, D\n", pc);
            b = d;
            pc++;
            cycles = 5;
            break;
        case 0x43:
            //printf("%02x   MOV B, E\n", pc);
            b = e;
            pc++;
            cycles = 5;
            break;
        case 0x44:
            //printf("%02x   MOV B, H\n", pc);
            b = h;
            pc++;
            cycles = 5;
            break;
        case 0x45:
            //printf("%02x   MOV B, L\n", pc);
            b = l;
            pc++;
            cycles = 5;
            break;
        case 0x46:
            //printf("%02x   MOV B, M\n", pc);
            b = memory[(h << 8) | l];
            pc++;
            cycles = 7;
            break;
        case 0x47:
            //printf("%02x   MOV B, A\n", pc);
            b = a;
            pc++;
            cycles = 5;
            break;
        case 0x48:
            //printf("%02x   MOV C, B\n", pc);
            c = b;
            pc++;
            cycles = 5;
            break;
        case 0x49:
            //printf("%02x   MOV C, C\n", pc);
            c = c;
            pc++;
            cycles = 5;
            break;
        case 0x4A:
            //printf("%02x   MOV C, D\n", pc);
            c = d;
            pc++;
            cycles = 5;
            break;
        case 0x4B:
            //printf("%02x   MOV C, E\n", pc);
            c = e;
            pc++;
            cycles = 5;
            break;
        case 0x4C:
            //printf("%02x   MOV C, H\n", pc);
            c = h;
            pc++;
            cycles = 5;
            break;
        case 0x4D:
            //printf("%02x   MOV C, L\n", pc);
            c = l;
            pc++;
            cycles = 5;
            break;
        case 0x4E:
            //printf("%02x   MOV C, M\n", pc);
            c = memory[(h << 8) | l];
            pc++;
            cycles = 7;
            break;
        case 0x4F:
            //printf("%02x   MOV C, A\n", pc);
            c = a;
            pc++;
            cycles = 5;
            break;
        
        case 0x50:
            //printf("%02x   MOV D, B\n", pc);
            d = b;
            pc++;
            cycles = 5;
            break;
        case 0x51:
            //printf("%02x   MOV D, C\n", pc);
            d = c;
            pc++;
            cycles = 5;
            break;
        case 0x52:
            //printf("%02x   MOV D, D\n", pc);
            d = d;
            pc++;
            cycles = 5;
            break;
        case 0x53:
            //printf("%02x   MOV D, E\n", pc);
            d = e;
            pc++;
            cycles = 5;
            break;
        case 0x54:
            //printf("%02x   MOV D, H\n", pc);
            d = h;
            pc++;
            cycles = 5;
            break;
        case 0x55:
            //printf("%02x   MOV D, L\n", pc);
            d = l;
            pc++;
            cycles = 5;
            break;
        case 0x56:
            //printf("%02x   MOV D, M\n", pc);
            d = memory[(h << 8) | l];
            pc++;
            cycles = 7;
            break;
        case 0x57:
            //printf("%02x   MOV D, A\n", pc);
            d = a;
            pc++;
            cycles = 5;
            break;
        case 0x58:
            //printf("%02x   MOV E, B\n", pc);
            e = b;
            pc++;
            cycles = 5;
            break;
        case 0x59:
            //printf("%02x   MOV E, C\n", pc);
            e = c;
            pc++;
            cycles = 5;
            break;
        case 0x5A:
            //printf("%02x   MOV E, D\n", pc);
            e = d;
            pc++;
            cycles = 5;
            break;
        case 0x5B:
            //printf("%02x   MOV E, E\n", pc);
            e = e; 
            pc++;
            cycles = 5;
            break;
        case 0x5C:
            //printf("%02x   MOV E, H\n", pc);
            e = h;
            pc++;
            cycles = 5;
            break;
        case 0x5D:
            //printf("%02x   MOV E, L\n", pc);
            e = l;
            pc++;
            cycles = 5;
            break;
        case 0x5E:
            //printf("%02x   MOV E, M\n", pc);
            e = memory[(h << 8) | l];
            pc++;
            cycles = 7;
            break;
        case 0x5F:
            //printf("%02x   MOV E, A\n", pc);
            e = a; 
            pc++;
            cycles = 5;
            break;
        
        case 0x60:
            //printf("%02x   MOV H, B\n", pc);
            h = b;
            pc++;
            cycles = 5;
            break;
        case 0x61:
            //printf("%02x   MOV H, C\n", pc);
            h = c;
            pc++;
            cycles = 5;
            break;
        case 0x62:
            //printf("%02x   MOV H, D\n", pc);
            h = d;
            pc++;
            cycles = 5;
            break;
        case 0x63:
            //printf("%02x   MOV H, E\n", pc);
            h = e;
            pc++;
            cycles = 5;
            break;
        case 0x64:
            //printf("%02x   MOV H, H\n", pc);
            h = h;
            pc++;
            cycles = 5;
            break;
        case 0x65:
            //printf("%02x   MOV H, L\n", pc);
            h = l;
            pc++;
            cycles = 5;
            break;
        case 0x66:
            //printf("%02x   MOV H, M\n", pc);
            h = memory[(h << 8) | l];
            pc++;
            cycles = 7;
            break;
        case 0x67:
            //printf("%02x   MOV H, A\n", pc);
            h = a;
            pc++;
            cycles = 5;
            break;
        case 0x68:
            //printf("%02x   MOV L, B\n", pc);
            l = b;
            pc++;
            cycles = 5;
            break;
        case 0x69:
            //printf("%02x   MOV L, C\n", pc);
            l = c;
            pc++;
            cycles = 5;
            break;
        case 0x6A:
            //printf("%02x   MOV L, D\n", pc);
            l = d;
            pc++;
            cycles = 5;
            break;
        case 0x6B:
            //printf("%02x   MOV L, E\n", pc);
            l = e;
            pc++;
            cycles = 5;
            break;
        case 0x6C:
            //printf("%02x   MOV L, H\n", pc);
            l = h;
            pc++;
            cycles = 5;
            break;
        case 0x6D:
            //printf("%02x   MOV L, L\n", pc);
            l = l;
            pc++;
            cycles = 5;
            break;
        case 0x6E:
            //printf("%02x   MOV L, M\n", pc);
            l = memory[(h << 8) | l];
            pc++;
            cycles = 7;
            break;
        case 0x6F:
            //printf("%02x   MOV L, A\n", pc);
            l = a;
            pc++;
            cycles = 5;
            break;
        
        case 0x70:
            //printf("%02x   MOV M, B\n", pc);
            memory[(h << 8) | l] = b;
            pc++;
            cycles = 7;
            break;
        case 0x71:
            //printf("%02x   MOV M, C\n", pc);
            memory[(h << 8) | l] = c;
            pc++;
            cycles = 7;
            break;
        case 0x72:
            //printf("%02x   MOV M, D\n", pc);
            memory[(h << 8) | l] = d;
            pc++;
            cycles = 7;
            break;
        case 0x73:
            //printf("%02x   MOV M, E\n", pc);
            memory[(h << 8) | l] = e;
            pc++;
            cycles = 7;
            break;
        case 0x74:
            //printf("%02x   MOV M, H\n", pc);
            memory[(h << 8) | l] = h;
            pc++;
            cycles = 7;
            break;
        case 0x75:
            //printf("%02x   MOV M, L\n", pc);
            memory[(h << 8) | l] = l;
            pc++;
            cycles = 7;
            break;
        case 0x76:
            //printf("%02x   HALT\n", pc);
            pc++;
            cycles = 7;
            break;
        case 0x77:
            //printf("%02x   MOV M, A\n", pc);
            memory[(h << 8) | l] = a;
            pc++;
            cycles = 7;
            break;
        case 0x78:
            //printf("%02x   MOV A, B\n", pc);
            a = b;
            pc++;
            cycles = 5;
            break;
        case 0x79:
            //printf("%02x   MOV A, C\n", pc);
            a = c;
            pc++;
            cycles = 5;
            break;
        case 0x7A:
            //printf("%02x   MOV A, D\n", pc);
            a = d;
            pc++;
            cycles = 5;
            break;
        case 0x7B:
            //printf("%02x   MOV A, E\n", pc);
            a = e;
            pc++;
            cycles = 5;
            break;
        case 0x7C:
            //printf("%02x   MOV A, H\n", pc);
            a = h;
            pc++;
            cycles = 5;
            break;
        case 0x7D:
            //printf("%02x   MOV A, L\n", pc);
            a = l;
            pc++;
            cycles = 5;
            break;
        case 0x7E:
            //printf("%02x   MOV A, M\n", pc);
            a = memory[(h << 8) | l]; 
            pc++;
            cycles = 7;
            break;
        case 0x7F:
            //printf("%02x   MOV A, A\n", pc);
            a = a; 
            pc++;
            cycles = 5;
            break;

        case 0x80:
            //printf("%02x   ADD B\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)b;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x81:
            //printf("%02x   ADD C\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)c;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x82:
            //printf("%02x   ADD D\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)d;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x83:
            //printf("%02x   ADD E\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)e;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x84:
            //printf("%02x   ADD H\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)h;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x85:
            //printf("%02x   ADD L\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)l;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x86:
            //printf("%02x   ADD M\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)memory[(h << 8) | l];
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 7;
            break;
        case 0x87:
            //printf("%02x   ADD A\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)a;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x88:
            //printf("%02x   ADC B\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)b + (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x89:
            //printf("%02x   ADC C\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)c + (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x8A:
            //printf("%02x   ADC D\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)d + (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x8B:
            //printf("%02x   ADC E\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)e + (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x8C:
            //printf("%02x   ADC H\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)h + (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x8D:
            //printf("%02x   ADC L\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)l + (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x8E:
            //printf("%02x   ADC M\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)memory[(h << 8) | l] + (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 7;
            break;
        case 0x8F:
            //printf("%02x   ADC A\n", pc);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)a + (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        
        case 0x90:
            //printf("%02x   SUB B\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)b;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x91:
            //printf("%02x   SUB C\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)c;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x92:
            //printf("%02x   SUB D\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)d;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x93:
            //printf("%02x   SUB E\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)e;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x94:
            //printf("%02x   SUB H\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)h;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x95:
            //printf("%02x   SUB L\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)l;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x96:
            //printf("%02x   SUB M\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)memory[(h << 8) | l];
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 7;
            break;
        case 0x97:
            //printf("%02x   SUB A\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)a;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x98:
            //printf("%02x   SBB B\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)b - (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x99:
            //printf("%02x   SBB C\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)c - (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x9A:
            //printf("%02x   SBB D\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)d - (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x9B:
            //printf("%02x   SBB E\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)e - (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x9C:
            //printf("%02x   SBB H\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)h - (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x9D:
            //printf("%02x   SBB L\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)l - (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0x9E:
            //printf("%02x   SBB M\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)memory[(h << 8) | l] - (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 7;
            break;
        case 0x9F:
            //printf("%02x   SBB A\n", pc);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)a - (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;

        case 0xA0:
            //printf("%02x   ANA B\n", pc);
            {
                __uint16_t result = (__uint16_t)a & (__uint16_t)b;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0xA1:
            //printf("%02x   ANA C\n", pc);
            {
                __uint16_t result = (__uint16_t)a & (__uint16_t)c;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0xA2:
            //printf("%02x   ANA D\n", pc);
            {
                __uint16_t result = (__uint16_t)a & (__uint16_t)d;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0xA3:
            //printf("%02x   ANA E\n", pc);
            {
                __uint16_t result = (__uint16_t)a & (__uint16_t)e;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0xA4:
            //printf("%02x   ANA H\n", pc);
            {
                __uint16_t result = (__uint16_t)a & (__uint16_t)h;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0xA5:
            //printf("%02x   ANA L\n", pc);
            {
                __uint16_t result = (__uint16_t)a & (__uint16_t)l;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0xA6:
            //printf("%02x   ANA M\n", pc);
            {
                __uint16_t result = (__uint16_t)a & (__uint16_t)memory[(h << 8) | l];
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 7;
            break;
        case 0xA7:
            //printf("%02x   ANA A\n", pc);
            {
                __uint16_t result = (__uint16_t)a & (__uint16_t)a;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc++;
            cycles = 4;
            break;
        case 0xA8:
            //printf("%02x   XRA B\n", pc);
            {
                __uint16_t result = (__uint16_t)a ^ (__uint16_t)b;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 4;
            break;
        case 0xA9:
            //printf("%02x   XRA C\n", pc);
            {
                __uint16_t result = (__uint16_t)a ^ (__uint16_t)c;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 4;
            break;
        case 0xAA:
            //printf("%02x   XRA D\n", pc);
            {
                __uint16_t result = (__uint16_t)a ^ (__uint16_t)d;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 4;
            break;
        case 0xAB:
            //printf("%02x   XRA E\n", pc);
            {
                __uint16_t result = (__uint16_t)a ^ (__uint16_t)e;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 4;
            break;
        case 0xAC:
            //printf("%02x   XRA H\n", pc);
            {
                __uint16_t result = (__uint16_t)a ^ (__uint16_t)h;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 4;
            break;
        case 0xAD:
            //printf("%02x   XRA L\n", pc);
            {
                __uint16_t result = (__uint16_t)a ^ (__uint16_t)l;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 4;
            break;
        case 0xAE:
            //printf("%02x   XRA M\n", pc);
            {
                __uint16_t result = (__uint16_t)a ^ (__uint16_t)memory[(h << 8) | l];
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 7;
            break;
        case 0xAF:
            //printf("%02x   XRA A\n", pc);
            {
                __uint16_t result = (__uint16_t)a ^ (__uint16_t)a;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 4;
            break;

        case 0xB0:
            //printf("%02x   ORA B\n", pc);
            {
                __uint16_t result = (__uint16_t)a | (__uint16_t)b;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 4;
            break;
        case 0xB1:
            //printf("%02x   ORA C\n", pc);
            {
                __uint16_t result = (__uint16_t)a | (__uint16_t)c;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 4;
            break;
        case 0xB2:
            //printf("%02x   ORA D\n", pc);
            {
                __uint16_t result = (__uint16_t)a | (__uint16_t)d;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 4;
            break;
        case 0xB3:
            //printf("%02x   ORA E\n", pc);
            {
                __uint16_t result = (__uint16_t)a | (__uint16_t)e;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 4;
            break;
        case 0xB4:
            //printf("%02x   ORA H\n", pc);
            {
                __uint16_t result = (__uint16_t)a | (__uint16_t)h;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 4;
            break;
        case 0xB5:
            //printf("%02x   ORA L\n", pc);
            {
                __uint16_t result = (__uint16_t)a | (__uint16_t)l;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 4;
            break;
        case 0xB6:
            //printf("%02x   ORA M\n", pc);
            {
                __uint16_t result = (__uint16_t)a | (__uint16_t)memory[(h << 8) | l];
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 7;
            break;
        case 0xB7:
            //printf("%02x   ORA A\n", pc);
            {
                __uint16_t result = (__uint16_t)a | (__uint16_t)a;
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc++;
            cycles = 4;
            break;
        case 0xB8:
            //printf("%02x   CMP B\n", pc);
            {
                __uint16_t result = a - b;
                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
                CF = (a < b);
            }
            pc++;
            cycles = 4;
            break;
        case 0xB9:
            //printf("%02x   CMP C\n", pc);
            {
                __uint16_t result = a - c;
                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
                CF = (a < c);
            }
            pc++;
            cycles = 4;
            break;
        case 0xBA:
            //printf("%02x   CMP D\n", pc);
            {
                __uint16_t result = a - d;
                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
                CF = (a < d);
            }
            pc++;
            cycles = 4;
            break;
        case 0xBB:
            //printf("%02x   CMP E\n", pc);
            {
                __uint16_t result = a - e;
                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
                CF = (a < e);
            }
            pc++;
            cycles = 4;
            break;
        case 0xBC:
            //printf("%02x   CMP H\n", pc);
            {
                __uint16_t result = a - h;
                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
                CF = (a < h);
            }
            pc++;
            cycles = 4;
            break;
        case 0xBD:
            //printf("%02x   CMP L\n", pc);
            {
                __uint16_t result = a - l;
                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
                CF = (a < l);
            }
            pc++;
            cycles = 4;
            break;
        case 0xBE:
            //printf("%02x   CMP M\n", pc);
            {
                __uint16_t result = a - memory[(h << 8) | l];
                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
                CF = (a < memory[(h << 8) | l]);
            }
            pc++;
            cycles = 7;
            break;
        case 0xBF:
            //printf("%02x   CMP A\n", pc);
            {
                __uint16_t result = a - a;
                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
                CF = (a < a);
            }
            pc++;
            cycles = 4;
            break;

        case 0xC0:
            //printf("%02x   RNZ\n", pc);
            if (ZF == 0){
                pc = (memory[sp + 1] << 8) | memory[sp];
                sp += 2;
            } else {
                pc++;
            }
            cycles = 11;
            break;
        case 0xC1:
            //printf("%02x   POP B\n", pc);
            b = memory[sp + 1];
            c = memory[sp];
            sp += 2;  
            pc++;
            cycles = 10;
            break;
        case 0xC2:
            //printf("%02x   JNZ %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (ZF == 0){
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }
            cycles = 10;
            break;
        case 0xC3:
            //printf("%02x   JMP %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            cycles = 10;
            break;
        case 0xC4:
            //printf("%02x   CNZ %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (ZF == 0){
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }
            cycles = 17;
            break;
        case 0xC5:
            //printf("%02x   PUSH B\n", pc);
            memory[sp - 2] = c;
            memory[sp - 1] = b;
            sp -= 2;
            pc++;
            cycles = 11;
            break;
        case 0xC6:
            //printf("%02x   ADI %02x\n", pc, memory[pc + 1]);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)memory[pc + 1];
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc += 2;
            cycles = 7;
            break;
        case 0xC7:
            //printf("%02x   RST 0\n", pc);
            {
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (0 << 8) | 0;
            }

            cycles = 11;
            break;
        case 0xC8:
            //printf("%02x   RZ\n", pc);
            if (ZF){
                pc = (memory[sp + 1] << 8) | memory[sp];
                sp += 2;
            } else {
                pc++;
            }
            cycles = 11;
            break;
        case 0xC9:
            //printf("%02x   RET\n", pc);
            pc = (memory[sp + 1] << 8) | memory[sp];
            sp += 2;
            cycles = 10;
            break;
        case 0xCA:
            //printf("%02x   JZ %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (ZF){
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }
            cycles = 10;
            break;
        case 0xCB:
            //printf("%02x   JMP %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            cycles = 10;
            break;
        case 0xCC:
            //printf("%02x   CZ %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (ZF){
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }
            cycles = 10;
            break;
        case 0xCD:
            //printf("%02x   CALL %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            {
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            }
            cycles = 17;
            break;
        case 0xCE:
            //printf("%02x   ACI %02x\n", pc, memory[pc + 1]);
            {
                __uint16_t result = (__uint16_t)a + (__uint16_t)memory[pc + 1] + (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc += 2;
            cycles = 7;
            break;
        case 0xCF:
            //printf("%02x   RST 1\n", pc);
            {
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (0 << 8) | 8;
            }
            cycles = 11;
            break;

        case 0xD0:
            //printf("%02x   RNC\n", pc);
            if (CF == 0){
                pc = (memory[sp + 1] << 8) | memory[sp];
                sp += 2;
            } else {
                pc++;
            }
            cycles = 11;
            break;
        case 0xD1:
            //printf("%02x   POP D\n", pc);
            d = memory[sp + 1];
            e = memory[sp];
            sp += 2;  
            pc++;
            cycles = 10;
            break;
        case 0xD2:
            //printf("%02x   JNC %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (CF == 0){
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }

            cycles = 10;
            break;

        case 0xD3:
            //printf("%02x   OUT %02x\n", pc, memory[pc + 1]);
            
            //getchar();

            //memory[memory[pc + 1]] = a;

            pc += 2;
            cycles = 10;
            break;

        case 0xD4:
            //printf("%02x   CNC %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (CF == 0){
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }
            cycles = 17;
            break;
        case 0xD5:
            //printf("%02x   PUSH D\n", pc);
            memory[sp - 2] = e;
            memory[sp - 1] = d;
            sp -= 2;
            pc++;
            cycles = 11;
            break;
        case 0xD6:
            //printf("%02x   SUI %02x\n", pc, memory[pc + 1]);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)memory[pc + 1];
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc += 2;
            cycles = 7;
            break;
        case 0xD7:
            //printf("%02x   RST 2\n", pc);
            {
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (0 << 8) | 16;
            }
            cycles = 11;
            break;
        case 0xD8:
            //printf("%02x   RC\n", pc);
            if (CF){
                pc = (memory[sp + 1] << 8) | memory[sp];
                sp += 2;
            } else {
                pc++;
            }
            cycles = 11;
            break;
        case 0xD9:
            //printf("%02x   RET\n", pc);
            pc = (memory[sp + 1] << 8) | memory[sp];
            sp += 2;
            cycles = 10;
            break;
        case 0xDA:
            //printf("%02x   JC %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (CF){
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }
            cycles = 10;
            break;

        case 0xDB:
            //printf("%02x   IN %02x\n", pc, memory[pc + 1]);
            switch (memory[pc + 1]){
                case 0:
                    a = 0b00001110;
                    break;
                case 1:
					a = 0b00001000;
                    break;
                case 2:
                    break;
                case 3:
                    break;
            }
            pc += 2;
            cycles = 10;
            break;

        case 0xDC:
            //printf("%02x   CC %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (CF){
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }
            cycles = 10;
            break;
        case 0xDD:
            //printf("%02x   CALL %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            {
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            }
            cycles = 17;
            break;
        case 0xDE:
            //printf("%02x   SBI %02x\n", pc, memory[pc + 1]);
            {
                __uint16_t result = (__uint16_t)a - (__uint16_t)memory[pc + 1] - (__uint16_t)CF;
                a = result & 0xff;
                
                CF = (result > 0xff);
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc += 2;
            cycles = 7;
            break;
        case 0xDF:
            //printf("%02x   RST 3\n", pc);
            {
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (0 << 8) | 24;
            }
            cycles = 11;
            break;

        case 0xE0:
            //printf("%02x   RPO\n", pc);
            if (PF == 0){
                pc = (memory[sp + 1] << 8) | memory[sp];
                sp += 2;
            } else {
                pc++;
            }
            cycles = 11;
            break;
        case 0xE1:
            //printf("%02x   POP H\n", pc);
            h = memory[sp + 1];
            l = memory[sp];
            sp += 2;  
            pc++;
            cycles = 10;
            break;
        case 0xE2:
            //printf("%02x   JPO %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (PF == 0){
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }
            cycles = 10;
            break;
        case 0xE3:
            //printf("%02x   XTHL\n", pc);
            {
                __uint8_t temph = h;
                __uint8_t templ = l;

                l = memory[sp];
                h = memory[sp + 1];

                memory[sp] = templ;
                memory[sp + 1] = temph;
            }
            pc++;

            cycles = 18;
            break;
        case 0xE4:
            //printf("%02x   CPO %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (PF == 0){
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }
            cycles = 17;
            break;
        case 0xE5:
            //printf("%02x   PUSH H\n", pc);
            memory[sp - 2] = l;
            memory[sp - 1] = h;
            sp -= 2;
            pc++;
            cycles = 11;
            break;
        case 0xE6:
            //printf("%02x   ANI %02x\n", pc, memory[pc + 1]);
            {
                __uint16_t result = (__uint16_t)a & (__uint16_t)memory[pc + 1];
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = (result > 0x09);
            }
            pc += 2;
            cycles = 7;
            break;
        case 0xE7:
            //printf("%02x   RST 4\n", pc);
            {
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (0 << 8) | 32;
            }
            cycles = 11;
            break;
        case 0xE8:
            //printf("%02x   RPE\n", pc);
            if (PF == 0){
                pc = (memory[sp + 1] << 8) | memory[sp];
                sp += 2;
            } else {
                pc++;
            }

            cycles = 11;
            break;
        case 0xE9:
            //printf("%02x   PCHL\n", pc);
            pc = (h << 8) | l;
            cycles = 5;
            break;
        case 0xEA:
            //printf("%02x   JPE %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (PF){
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }
            cycles = 10;
            break;
        case 0xEB:
            //printf("%02x   XCHG\n", pc);
            {
                __uint8_t tempd = d;
                __uint8_t tempe = e;

                d = h;
                e = l;

                h = tempd;
                l = tempe;
            }
            pc++;
            cycles = 5;
            break;
        case 0xEC:
            //printf("%02x   CPE %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (PF){
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }
            cycles = 10;
            break;
        case 0xED:
            //printf("%02x   CALL %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            {
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            }
            cycles = 17;
            break;
        case 0xEE:
            //printf("%02x   XRI %02x\n", pc, memory[pc + 1]);
            {
                __uint16_t result = (__uint16_t)a ^ (__uint16_t)memory[pc + 1];
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc += 2;
            cycles = 7;
            break;
        case 0xEF:
            //printf("%02x   RST 5\n", pc);
            {
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (0 << 8) | 40;
            }
            cycles = 11;
            break;

        case 0xF0:
            //printf("%02x   RP\n", pc);
            if (SF == 0){
                pc = (memory[sp + 1] << 8) | memory[sp];
                sp += 2;
            } else {
                pc++;
            }
            cycles = 11;
            break;
        case 0xF1:
            //printf("%02x   POP PSW\n", pc);
            {
                __uint8_t PSW = memory[sp];
                CF = ((PSW & 0x1) != 0);
                PF = ((PSW & 0x4) != 0);
                AF = ((PSW & 0x10) != 0);
                ZF = ((PSW & 0x40) != 0);
                SF = ((PSW & 0x80) != 0);
                a = memory[sp + 1];
                sp += 2;
            }
            pc++;
            cycles = 10;
            break;
        case 0xF2:
            //printf("%02x   JP %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (SF == 0){
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }
            cycles = 10;
            break;

        case 0xF3:
            //printf("%02x   DI\n", pc);
            int_enable = 0;
            pc++;
            cycles = 4;
            break;

        case 0xF4:
            //printf("%02x   CP %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (SF == 0){
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }
            cycles = 17;
            break;
        case 0xF5:
            //printf("%02x   PUSH PSW\n", pc);
            {
                memory[sp - 2] = 
                    (CF & 0x01) |
                    (0x02) |
                    (CF << 2) |
                    (AF << 4) | 
                    (ZF << 6) |
                    (SF << 7) |
                    (0x00);

                memory[sp - 1] = a;
                sp -= 2;
            }
            pc++;
            cycles = 11;
            break;
        case 0xF6:
            //printf("%02x   ORI %02x\n", pc, memory[pc + 1]);
            {
                __uint16_t result = (__uint16_t)a | (__uint16_t)memory[pc + 1];
                a = result;

                CF = 0;
                ZF = ((result & 0xff) == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result & 0xff, 8);
                AF = 0;
            }
            pc += 2;
            cycles = 7;
            break;
        case 0xF7:
            //printf("%02x   RST 6\n", pc);
            {
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (0 << 8) | 48;
            }
            cycles = 11;
            break;
        case 0xF8:
            //printf("%02x   RM\n", pc);
            if (SF){
                pc = (memory[sp + 1] << 8) | memory[sp];
                sp += 2;
            } else {
                pc++;
            }

            cycles = 11;
            break;
        case 0xF9:
            //printf("%02x   SPHL\n", pc);
            sp = (h << 8) | l;
            pc++;

            cycles = 5;
            break;
        case 0xFA:
            //printf("%02x   JM %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (SF){
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }

            cycles = 10;
            break;

        case 0xFB:
            //printf("%02x   EI\n", pc);
            int_enable = 1;
            pc++;
            cycles = 4;
            break;

        case 0xFC:
            //printf("%02x   CM %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            if (SF){
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            } else {
                pc += 3;
            }
            cycles = 10;
            break;
        case 0xFD:
            //printf("%02x   CALL %02x%02x\n", pc, memory[pc + 2], memory[pc + 1]);
            {
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (memory[pc + 2] << 8) | (memory[pc + 1]);
            }

            cycles = 17;
            break;
        case 0xFE:
            //printf("%02x   CPI %02x\n", pc, memory[pc + 1]);
            {
                __uint16_t result = a - memory[pc + 1];
                ZF = (result == 0);
                SF = (0x80 == (result & 0x80));
                PF = parity(result, 8);
                CF = (a < memory[pc + 1]);
            }
            cycles = 7;
            pc += 2;
            break;
        case 0xFF:
            //printf("%02x   RST 7\n", pc);
            {
                __uint16_t ret = pc + 3;
                memory[sp - 1] = (ret >> 8) & 0xFF;
                memory[sp - 2] = ret & 0xFF;
                sp -= 2;
                pc = (0 << 8) | 56;
            }
            cycles = 11;
            break;

        default:
            //printf("%02x   Unimplemented instruction %02x\n", pc, memory[pc]);
            pc++;
            break;
    }

    return cycles;
}

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
    int whichInterrupt;

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
				uint8_t byte = memory[videomemory + index];

				for (int b = 0; b < 8; b++) {
					SDL_Rect squareRect;
					squareRect.w = RECTANGLE_SIZE;
					squareRect.h = RECTANGLE_SIZE;
                    squareRect.x = ix * RECTANGLE_SIZE;
					squareRect.y = (HEIGHT - iy - b) * RECTANGLE_SIZE;

					if ((byte & 0x1) != 0){
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
