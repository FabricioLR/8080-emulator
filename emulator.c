#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parity(int x, int size){
	int parity = 0;

	int i;
	for (i = 0; i < size; i++){
		parity += x & 1;
		x = x >> 1;
	}

	return (parity % 2 == 0);
}

int main() { 
    FILE *file, *file1, *file2, *file3;
    int char1;
    long length = 0;

    file = fopen("invaders.h", "rb");
    file1 = fopen("invaders.g", "rb");
    file2 = fopen("invaders.f", "rb");
    file3 = fopen("invaders.e", "rb");

    unsigned char memory[0x10000];

    __uint8_t flags = 0;

    __uint16_t i = 0;
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
    int whichInterrupt = 1;


    while (1){
        char1 = fgetc(file);

        if (char1 == EOF){
            break;
        }

        memory[i] = char1;
        i++;
    }

    while (1){
        char1 = fgetc(file1);
        if (char1 == EOF){
            break;
        }
        memory[i] = char1;
        i++;
    }

    while (1){
        char1 = fgetc(file2);
        if (char1 == EOF){
            break;
        }
        memory[i] = char1;
        i++;
    }

    while (1){
        char1 = fgetc(file3);
        if (char1 == EOF){
            break;
        }
        memory[i] = char1;
        i++;
    }

    fclose(file);
    fclose(file1);
    fclose(file2);
    fclose(file3);

    length = i;

    i = 0;
    while (i < length){
        //printf("%02x\n", memory[i]);
        if (int_enable){
            if (whichInterrupt == 1){
                
                memory[sp - 2] = (i + 1) & 0xff;
                memory[sp - 1] = ((i + 1) & 0xff00) >> 8;
                sp -= 2;
                
                i = 8 * 1;
                int_enable = 0;
                whichInterrupt = 2;
            } else {
                memory[sp - 2] = (i + 1) & 0xff;
                memory[sp - 1] = ((i + 1) & 0xff00) >> 8;
                sp -= 2;
                
                i = 8 * 2;
                int_enable = 0;
                whichInterrupt = 1;
            }
        }

        switch (memory[i]){
            case 0x00:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x01:
                printf("%02x   LXI B, %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                b = memory[i + 2];
                c = memory[i + 1];
                i += 3;
                break;
            case 0x02:
                printf("%02x   STAX B\n", i);
                memory[(b << 8) | c] = a;
                i++;
                break;
            case 0x03:
                printf("%02x   INX B\n", i);
                c++;
                if (c == 0) b++;
                i++;
                break;
            case 0x04:
                printf("%02x   INR B\n", i);
                {
                    __uint16_t result = (__uint16_t)b + 1;
                    b = result & 0x00FF;

                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80)); //res & 1000 0000
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09); //checks if result is greater than 9
                }
                i++;
                break;
            case 0x05:
                printf("%02x   DCR B\n", i);
                {
                    __uint16_t result = (__uint16_t)b - 1;
                    b = result & 0xFF;

                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                }
                i++;
                break;
            case 0x06:
                printf("%02x   MVI B, %02x\n", i, memory[i + 1]);
                b = memory[i + 1];
                i += 2;
                break;
            case 0x07:
                printf("%02x   RLC\n", i);
                {
                    __uint8_t temp = a;
                    a = temp << 1 | temp >> 7;
                    CF = ((temp >> 7) > 0);
                }
                i++;
                break;
            case 0x08:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x09:
                printf("%02x   DAD B\n", i);
                {
                    __uint32_t hl = (h << 8) | l;
                    __uint32_t bc = (b << 8) | c;
                    __uint32_t result = (hl + bc);

                    CF = ((result & 0xffff0000) > 0);

                    h = (result & 0xff00) >> 8;
                    l = (result & 0xff);
                }
                i++;
                break;
            case 0x0A:
                printf("%02x   LDAX B\n", i);
                a = memory[(b << 8) | c];
                i++;
                break;
            case 0x0B:
                printf("%02x   DCX B\n", i);
                c--;
                if (c == 0xff) b--;
                i++;
                break;
            case 0x0C:
                printf("%02x   INR C\n", i);
                {
                    __uint16_t result = (__uint16_t)c + 1;
                    c = result & 0x00FF;

                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80)); //res & 1000 0000
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09); //checks if result is greater than 9
                }
                i++;
                break;
            case 0x0D:
                printf("%02x   DCR C\n", i);
                {
                    __uint16_t result = (__uint16_t)c - 1;
                    c = result & 0xFF;

                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                }
                i++;
                break;
            case 0x0E:
                printf("%02x   MVI C, %02x\n", i, memory[i + 1]);
                c = memory[i + 1];
                i += 2;
                break;
            case 0x0F:
                printf("%02x   RRC\n", i);
                {
                    __uint8_t temp = a;
                    a = temp >> 1 | temp << 7;

                    CF = ((a >> 7) > 0);
                }
                i++;
                break;

            case 0x10:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x11:
                printf("%02x   LXI D, %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                d = memory[i + 2];
                e = memory[i + 1];
                i += 3;
                break;
            case 0x12:
                printf("%02x   STAX D\n", i);
                memory[(d << 8) | e] = a;
                i++;
                break;
            case 0x13:
                printf("%02x   INX D\n", i);
                e++;
                if (e == 0) d++;
                i++;
                break;
            case 0x14:
                printf("%02x   INR D\n", i);
                {
                    __uint16_t result = (__uint16_t)d + 1;
                    d = result & 0x00FF;

                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80)); //res & 1000 0000
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09); //checks if result is greater than 9
                }
                i++;
                break;
            case 0x15:
                printf("%02x   DCR D\n", i);
                {
                    __uint16_t result = (__uint16_t)d - 1;
                    d = result & 0xFF;

                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                }
                i++;
                break;
            case 0x16:
                printf("%02x   MVI D, %02x\n", i, memory[i + 1]);
                d = memory[i + 1];
                i += 2;
                break;
            case 0x17:
                printf("%02x   RAL\n", i);
                {
                    __uint8_t temp = a;
                    __uint8_t msb = (a >> 7);
                    a = (temp << 1) | (CF);
                    CF = msb;
                }
                i++;
                break;
            case 0x18:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x19:
                printf("%02x   DAD D\n", i);
                {
                    __uint16_t hl = (h << 8) | l;
                    __uint16_t de = (d << 8) | e;
                    __uint32_t result = (hl + de);

                    CF = ((result & 0xffff0000) > 0);

                    h = (result & 0xff00) >> 8;
                    l = (result & 0xff);
                }
                i++;
                break;
            case 0x1A:
                printf("%02x   LDAX D\n", i);
                a = memory[(d << 8) | e];
                i++;
                break;
            case 0x1B:
                printf("%02x   DCX D\n", i);
                e--;
                if (e == 0xff) d--;
                i++;
                break;
            case 0x1C:
                printf("%02x   INR E\n", i);
                {
                    __uint16_t result = (__uint16_t)e + 1;
                    e = result & 0x00FF;

                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80)); //res & 1000 0000
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09); //checks if result is greater than 9
                }
                i++;
                break;
            case 0x1D:
                printf("%02x   DCR E\n", i);
                {
                    __uint16_t result = (__uint16_t)e - 1;
                    e = result & 0xFF;

                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                }
                i++;
                break;
            case 0x1E:
                printf("%02x   MVI E, %02x\n", i, memory[i + 1]);
                e = memory[i + 1];
                i += 2;
                break;
            case 0x1F:
                printf("%02x   RAR\n", i);
                {
                    __uint8_t temp = a;
                    __uint8_t msb = ((a >> 7) << 7);
                    a = (temp >> 1) | (msb);
                    CF = (temp << 7) >> 7;
                }
                i++;
                break;
            
            case 0x20:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x21:
                printf("%02x   LXI H, %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                h = memory[i + 2];
                l = memory[i + 1];
                i += 3;
                break;
            case 0x22:
                printf("%02x   SHLD %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                memory[(memory[i + 2] << 8) | memory[i + 1]] = l;
                memory[((memory[i + 2] << 8) | memory[i + 1]) + 1] = h;
                i += 3;
                break;
            case 0x23:
                printf("%02x   INX H\n", i);
                l++;
                if (l == 0) h++;
                i++;
                break;
            case 0x24:
                printf("%02x   INR H\n", i);
                {
                    __uint16_t result = (__uint16_t)h + 1;
                    h = result & 0x00FF;

                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80)); //res & 1000 0000
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09); //checks if result is greater than 9
                }
                i++;
                break;
            case 0x25:
                printf("%02x   DCR H\n", i);
                {
                    __uint16_t result = (__uint16_t)h - 1;
                    h = result & 0xFF;

                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                }
                i++;
                break;
            case 0x26:
                printf("%02x   MVI H, %02x\n", i, memory[i + 1]);
                h = memory[i + 1];
                i += 2;
                break;
            case 0x27:
                printf("%02x   DAA\n", i);
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
                i++;
                break;
            case 0x28:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x29:
                printf("%02x   DAD H\n", i);
                {
                    __uint16_t hl = (h << 8) | l;
                    __uint32_t result = (hl + hl);

                    CF = ((result & 0xffff0000) > 0);

                    h = (result & 0xff00) >> 8;
                    l = (result & 0xff);
                }
                i++;
                break;
            case 0x2A:
                printf("%02x   LHLD %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                l = memory[(memory[i + 2] << 8) | memory[i + 1]];
                h = memory[((memory[i + 2] << 8) | memory[i + 1]) + 1];
                i += 3;
                break;
            case 0x2B:
                printf("%02x   DCX H\n", i);
                l--;
                if (l == 0xff) h--;
                i++;
                break;
            case 0x2C:
                printf("%02x   INR L\n", i);
                {
                    __uint16_t result = (__uint16_t)l + 1;
                    l = result & 0x00FF;

                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80)); //res & 1000 0000
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09); //checks if result is greater than 9
                }
                i++;
                break;
            case 0x2D:
                printf("%02x   DCR L\n", i);
                {
                    __uint16_t result = (__uint16_t)l - 1;
                    l = result & 0xFF;

                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                }
                i++;
                break;
            case 0x2E:
                printf("%02x   MVI L, %02x\n", i, memory[i + 1]);
                l = memory[i + 1];
                i += 2;
                break;
            case 0x2F:
                printf("%02x   CMA\n", i);
                a = ~(a);
                i++;
                break;
            
            case 0x30:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x31:
                printf("%02x   LXI SP, %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                sp = (memory[i + 2] << 8) | (memory[i + 1]);
                i += 3;
                break;
            case 0x32:
                printf("%02x   STA %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                memory[(memory[i + 2] << 8) | memory[i + 1]] = a;
                i += 3;
                break;
            case 0x33:
                printf("%02x   INX SP\n", i);
                sp++;
                i++;
                break;
            case 0x34:
                printf("%02x   INR M\n", i);
                {
                    __uint16_t result = (__uint16_t)memory[(h << 8) | l] + 1;
                    memory[(h << 8) | l] = result & 0x00FF;

                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x35:
                printf("%02x   DCR M\n", i);
                {
                    __uint16_t result = (__uint16_t)memory[(h << 8) | l] - 1;
                    memory[(h << 8) | l] = result & 0xFF;

                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                }
                i++;
                break;
            case 0x36:
                printf("%02x   MVI M, %02x\n", i, memory[i + 1]);
                memory[(h << 8) | l] = memory[i + 1];
                i += 2;
                break;
            case 0x37:
                printf("%02x   STC\n", i);
                CF = 1;
                i++;
                break;
            case 0x38:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x39:
                printf("%02x   DAD SP\n", i);
                {
                    __uint16_t hl = (h << 8) | l;
                    __uint32_t result = (hl + sp);

                    CF = ((result & 0xffff0000) > 0);

                    h = (result & 0xff00) >> 8;
                    l = (result & 0xff);
                }
                i++;
                break;
            case 0x3A:
                printf("%02x   LDA %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                a = memory[(memory[i + 2] << 8) | memory[i + 1]];
                i += 3;
                break;
            case 0x3B:
                printf("%02x   DCX SP\n", i);
                sp--;
                i++;
                break;
            case 0x3C:
                printf("%02x   INR A\n", i);
                {
                    __uint16_t result = (__uint16_t)a + 1;
                    a = result & 0x00FF;

                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80)); //res & 1000 0000
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09); //checks if result is greater than 9
                }
                i++;
                break;
            case 0x3D:
                printf("%02x   DCR A\n", i);
                {
                    __uint16_t result = (__uint16_t)a - 1;
                    a = result & 0xFF;

                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                }
                i++;
                break;
            case 0x3E:
                printf("%02x   MVI A, %02x\n", i, memory[i + 1]);
                a = memory[i + 1];
                i += 2;
                break;
            case 0x3F:
                printf("%02x   CMC\n", i);
                CF = ~(CF);
                i++;
                break;




            
            case 0x40:
                printf("%02x   MOV B, B\n", i);
                b = b;
                i++;
                break;
            case 0x41:
                printf("%02x   MOV B, C\n", i);
                b = c;
                i++;
                break;
            case 0x42:
                printf("%02x   MOV B, D\n", i);
                b = d;
                i++;
                break;
            case 0x43:
                printf("%02x   MOV B, E\n", i);
                b = e;
                i++;
                break;
            case 0x44:
                printf("%02x   MOV B, H\n", i);
                b = h;
                i++;
                break;
            case 0x45:
                printf("%02x   MOV B, L\n", i);
                b = l;
                i++;
                break;
            case 0x46:
                printf("%02x   MOV B, M\n", i);
                b = memory[(h << 8) | l];
                i++;
                break;
            case 0x47:
                printf("%02x   MOV B, A\n", i);
                b = a;
                i++;
                break;
            case 0x48:
                printf("%02x   MOV C, B\n", i);
                c = b;
                i++;
                break;
            case 0x49:
                printf("%02x   MOV C, C\n", i);
                c = c;
                i++;
                break;
            case 0x4A:
                printf("%02x   MOV C, D\n", i);
                c = d;
                i++;
                break;
            case 0x4B:
                printf("%02x   MOV C, E\n", i);
                c = e;
                i++;
                break;
            case 0x4C:
                printf("%02x   MOV C, H\n", i);
                c = h;
                i++;
                break;
            case 0x4D:
                printf("%02x   MOV C, L\n", i);
                c = l;
                i++;
                break;
            case 0x4E:
                printf("%02x   MOV C, M\n", i);
                c = memory[(h << 8) | l];
                i++;
                break;
            case 0x4F:
                printf("%02x   MOV C, A\n", i);
                c = a;
                i++;
                break;
            
            case 0x50:
                printf("%02x   MOV D, B\n", i);
                d = b;
                i++;
                break;
            case 0x51:
                printf("%02x   MOV D, C\n", i);
                d = c;
                i++;
                break;
            case 0x52:
                printf("%02x   MOV D, D\n", i);
                d = d;
                i++;
                break;
            case 0x53:
                printf("%02x   MOV D, E\n", i);
                d = e;
                i++;
                break;
            case 0x54:
                printf("%02x   MOV D, H\n", i);
                d = h;
                i++;
                break;
            case 0x55:
                printf("%02x   MOV D, L\n", i);
                d = l;
                i++;
                break;
            case 0x56:
                printf("%02x   MOV D, M\n", i);
                d = memory[(h << 8) | l];
                i++;
                break;
            case 0x57:
                printf("%02x   MOV D, A\n", i);
                d = a;
                i++;
                break;
            case 0x58:
                printf("%02x   MOV E, B\n", i);
                e = b;
                i++;
                break;
            case 0x59:
                printf("%02x   MOV E, C\n", i);
                e = c;
                i++;
                break;
            case 0x5A:
                printf("%02x   MOV E, D\n", i);
                e = d;
                i++;
                break;
            case 0x5B:
                printf("%02x   MOV E, E\n", i);
                e = e; 
                i++;
                break;
            case 0x5C:
                printf("%02x   MOV E, H\n", i);
                e = h;
                i++;
                break;
            case 0x5D:
                printf("%02x   MOV E, L\n", i);
                e = l;
                i++;
                break;
            case 0x5E:
                printf("%02x   MOV E, M\n", i);
                e = memory[(h << 8) | l];
                i++;
                break;
            case 0x5F:
                printf("%02x   MOV E, A\n", i);
                e = a; 
                i++;
                break;
            
            case 0x60:
                printf("%02x   MOV H, B\n", i);
                h = b;
                i++;
                break;
            case 0x61:
                printf("%02x   MOV H, C\n", i);
                h = c;
                i++;
                break;
            case 0x62:
                printf("%02x   MOV H, D\n", i);
                h = d;
                i++;
                break;
            case 0x63:
                printf("%02x   MOV H, E\n", i);
                h = e;
                i++;
                break;
            case 0x64:
                printf("%02x   MOV H, H\n", i);
                h = h;
                i++;
                break;
            case 0x65:
                printf("%02x   MOV H, L\n", i);
                h = l;
                i++;
                break;
            case 0x66:
                printf("%02x   MOV H, M\n", i);
                h = memory[(h << 8) | l];
                i++;
                break;
            case 0x67:
                printf("%02x   MOV H, A\n", i);
                h = a;
                i++;
                break;
            case 0x68:
                printf("%02x   MOV L, B\n", i);
                l = b;
                i++;
                break;
            case 0x69:
                printf("%02x   MOV L, C\n", i);
                l = c;
                i++;
                break;
            case 0x6A:
                printf("%02x   MOV L, D\n", i);
                l = d;
                i++;
                break;
            case 0x6B:
                printf("%02x   MOV L, E\n", i);
                l = e;
                i++;
                break;
            case 0x6C:
                printf("%02x   MOV L, H\n", i);
                l = h;
                i++;
                break;
            case 0x6D:
                printf("%02x   MOV L, L\n", i);
                l = l;
                i++;
                break;
            case 0x6E:
                printf("%02x   MOV L, M\n", i);
                l = memory[(h << 8) | l];
                i++;
                break;
            case 0x6F:
                printf("%02x   MOV L, A\n", i);
                l = a;
                i++;
                break;
            
            case 0x70:
                printf("%02x   MOV M, B\n", i);
                memory[(h << 8) | l] = b;
                i++;
                break;
            case 0x71:
                printf("%02x   MOV M, C\n", i);
                memory[(h << 8) | l] = c;
                i++;
                break;
            case 0x72:
                printf("%02x   MOV M, D\n", i);
                memory[(h << 8) | l] = d;
                i++;
                break;
            case 0x73:
                printf("%02x   MOV M, E\n", i);
                memory[(h << 8) | l] = e;
                i++;
                break;
            case 0x74:
                printf("%02x   MOV M, H\n", i);
                memory[(h << 8) | l] = h;
                i++;
                break;
            case 0x75:
                printf("%02x   MOV M, L\n", i);
                memory[(h << 8) | l] = l;
                i++;
                break;
            case 0x76:
                printf("%02x   HALT\n", i);
                i++;
                break;
            case 0x77:
                printf("%02x   MOV M, A\n", i);
                memory[(h << 8) | l] = a;
                i++;
                break;
            case 0x78:
                printf("%02x   MOV A, B\n", i);
                a = b;
                i++;
                break;
            case 0x79:
                printf("%02x   MOV A, C\n", i);
                a = c;
                i++;
                break;
            case 0x7A:
                printf("%02x   MOV A, D\n", i);
                a = d;
                i++;
                break;
            case 0x7B:
                printf("%02x   MOV A, E\n", i);
                a = e;
                i++;
                break;
            case 0x7C:
                printf("%02x   MOV A, H\n", i);
                a = h;
                i++;
                break;
            case 0x7D:
                printf("%02x   MOV A, L\n", i);
                a = l;
                i++;
                break;
            case 0x7E:
                printf("%02x   MOV A, M\n", i);
                a = memory[(h << 8) | l]; 
                i++;
                break;
            case 0x7F:
                printf("%02x   MOV A, A\n", i);
                a = a; 
                i++;
                break;




            case 0x80:
                printf("%02x   ADD B\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)b;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x81:
                printf("%02x   ADD C\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)c;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x82:
                printf("%02x   ADD D\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)d;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x83:
                printf("%02x   ADD E\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)e;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x84:
                printf("%02x   ADD H\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)h;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x85:
                printf("%02x   ADD L\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)l;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x86:
                printf("%02x   ADD M\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)memory[(h << 8) | l];
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x87:
                printf("%02x   ADD A\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)a;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x88:
                printf("%02x   ADC B\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)b + (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x89:
                printf("%02x   ADC C\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)c + (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x8A:
                printf("%02x   ADC D\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)d + (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x8B:
                printf("%02x   ADC E\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)e + (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x8C:
                printf("%02x   ADC H\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)h + (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x8D:
                printf("%02x   ADC L\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)l + (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x8E:
                printf("%02x   ADC M\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)memory[(h << 8) | l] + (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x8F:
                printf("%02x   ADC A\n", i);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)a + (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            
            case 0x90:
                printf("%02x   SUB B\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)b;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x91:
                printf("%02x   SUB C\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)c;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x92:
                printf("%02x   SUB D\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)d;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x93:
                printf("%02x   SUB E\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)e;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x94:
                printf("%02x   SUB H\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)h;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x95:
                printf("%02x   SUB L\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)l;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x96:
                printf("%02x   SUB M\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)memory[(h << 8) | l];
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x97:
                printf("%02x   SUB A\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)a;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x98:
                printf("%02x   SBB B\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)b - (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x99:
                printf("%02x   SBB C\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)c - (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x9A:
                printf("%02x   SBB D\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)d - (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x9B:
                printf("%02x   SBB E\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)e - (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x9C:
                printf("%02x   SBB H\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)h - (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x9D:
                printf("%02x   SBB L\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)l - (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x9E:
                printf("%02x   SBB M\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)memory[(h << 8) | l] - (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0x9F:
                printf("%02x   SBB A\n", i);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)a - (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;

            case 0xA0:
                printf("%02x   ANA B\n", i);
                {
                    __uint16_t result = (__uint16_t)a & (__uint16_t)b;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0xA1:
                printf("%02x   ANA C\n", i);
                {
                    __uint16_t result = (__uint16_t)a & (__uint16_t)c;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0xA2:
                printf("%02x   ANA D\n", i);
                {
                    __uint16_t result = (__uint16_t)a & (__uint16_t)d;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0xA3:
                printf("%02x   ANA E\n", i);
                {
                    __uint16_t result = (__uint16_t)a & (__uint16_t)e;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0xA4:
                printf("%02x   ANA H\n", i);
                {
                    __uint16_t result = (__uint16_t)a & (__uint16_t)h;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0xA5:
                printf("%02x   ANA L\n", i);
                {
                    __uint16_t result = (__uint16_t)a & (__uint16_t)l;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0xA6:
                printf("%02x   ANA M\n", i);
                {
                    __uint16_t result = (__uint16_t)a & (__uint16_t)memory[(h << 8) | l];
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0xA7:
                printf("%02x   ANA A\n", i);
                {
                    __uint16_t result = (__uint16_t)a & (__uint16_t)a;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i++;
                break;
            case 0xA8:
                printf("%02x   XRA B\n", i);
                {
                    __uint16_t result = (__uint16_t)a ^ (__uint16_t)b;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xA9:
                printf("%02x   XRA C\n", i);
                {
                    __uint16_t result = (__uint16_t)a ^ (__uint16_t)c;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xAA:
                printf("%02x   XRA D\n", i);
                {
                    __uint16_t result = (__uint16_t)a ^ (__uint16_t)d;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xAB:
                printf("%02x   XRA E\n", i);
                {
                    __uint16_t result = (__uint16_t)a ^ (__uint16_t)e;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xAC:
                printf("%02x   XRA H\n", i);
                {
                    __uint16_t result = (__uint16_t)a ^ (__uint16_t)h;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xAD:
                printf("%02x   XRA L\n", i);
                {
                    __uint16_t result = (__uint16_t)a ^ (__uint16_t)l;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xAE:
                printf("%02x   XRA M\n", i);
                {
                    __uint16_t result = (__uint16_t)a ^ (__uint16_t)memory[(h << 8) | l];
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xAF:
                printf("%02x   XRA A\n", i);
                {
                    __uint16_t result = (__uint16_t)a ^ (__uint16_t)a;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;

            case 0xB0:
                printf("%02x   ORA B\n", i);
                {
                    __uint16_t result = (__uint16_t)a | (__uint16_t)b;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xB1:
                printf("%02x   ORA C\n", i);
                {
                    __uint16_t result = (__uint16_t)a | (__uint16_t)c;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xB2:
                printf("%02x   ORA D\n", i);
                {
                    __uint16_t result = (__uint16_t)a | (__uint16_t)d;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xB3:
                printf("%02x   ORA E\n", i);
                {
                    __uint16_t result = (__uint16_t)a | (__uint16_t)e;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xB4:
                printf("%02x   ORA H\n", i);
                {
                    __uint16_t result = (__uint16_t)a | (__uint16_t)h;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xB5:
                printf("%02x   ORA L\n", i);
                {
                    __uint16_t result = (__uint16_t)a | (__uint16_t)l;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xB6:
                printf("%02x   ORA M\n", i);
                {
                    __uint16_t result = (__uint16_t)a | (__uint16_t)memory[(h << 8) | l];
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xB7:
                printf("%02x   ORA A\n", i);
                {
                    __uint16_t result = (__uint16_t)a | (__uint16_t)a;
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i++;
                break;
            case 0xB8:
                printf("%02x   CMP B\n", i);
                {
                    __uint16_t result = a - b;
                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                    CF = (a < b);
                }
                i++;
                break;
            case 0xB9:
                printf("%02x   CMP C\n", i);
                {
                    __uint16_t result = a - c;
                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                    CF = (a < c);
                }
                i++;
                break;
            case 0xBA:
                printf("%02x   CMP D\n", i);
                {
                    __uint16_t result = a - d;
                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                    CF = (a < d);
                }
                i++;
                break;
            case 0xBB:
                printf("%02x   CMP E\n", i);
                {
                    __uint16_t result = a - e;
                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                    CF = (a < e);
                }
                i++;
                break;
            case 0xBC:
                printf("%02x   CMP H\n", i);
                {
                    __uint16_t result = a - h;
                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                    CF = (a < h);
                }
                i++;
                break;
            case 0xBD:
                printf("%02x   CMP L\n", i);
                {
                    __uint16_t result = a - l;
                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                    CF = (a < l);
                }
                i++;
                break;
            case 0xBE:
                printf("%02x   CMP M\n", i);
                {
                    __uint16_t result = a - memory[(h << 8) | l];
                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                    CF = (a < memory[(h << 8) | l]);
                }
                i++;
                break;
            case 0xBF:
                printf("%02x   CMP A\n", i);
                {
                    __uint16_t result = a - a;
                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                    CF = (a < a);
                }
                i++;
                break;

            case 0xC0:
                printf("%02x   RNZ\n", i);
                if (ZF == 0){
                    i = (memory[sp + 1] << 8) | memory[sp];
                    sp += 2;
                } else {
                    i++;
                }
                break;
            case 0xC1:
                printf("%02x   POP B\n", i);
                b = memory[sp + 1];
                c = memory[sp];
                sp += 2;  
                i++;
                break;
            case 0xC2:
                printf("%02x   JNZ %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (ZF == 0){
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;
            case 0xC3:
                printf("%02x   JMP %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                i = (memory[i + 2] << 8) | (memory[i + 1]);
                break;
            case 0xC4:
                printf("%02x   CNZ %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (ZF == 0){
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;
            case 0xC5:
                printf("%02x   PUSH B\n", i);
                memory[sp - 2] = c;
                memory[sp - 1] = b;
                sp -= 2;
                i++;
                break;
            case 0xC6:
                printf("%02x   ADI %02x\n", i, memory[i + 1]);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)memory[i + 1];
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i += 2;
                break;
            case 0xC7:
                printf("%02x   RST 0\n", i);
                {
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (0 << 8) | 0;
                }
                break;
            case 0xC8:
                printf("%02x   RZ\n", i);
                if (ZF){
                    i = (memory[sp + 1] << 8) | memory[sp];
                    sp += 2;
                } else {
                    i++;
                }
                break;
            case 0xC9:
                printf("%02x   RET\n", i);
                i = (memory[sp + 1] << 8) | memory[sp];
                sp += 2;
                break;
            case 0xCA:
                printf("%02x   JZ %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (ZF){
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;
            case 0xCB:
                printf("%02x   JMP %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                i = (memory[i + 2] << 8) | (memory[i + 1]);
                break;
            case 0xCC:
                printf("%02x   CZ %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (ZF){
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;
            case 0xCD:
                printf("%02x   CALL %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                {
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                }
                break;
            case 0xCE:
                printf("%02x   ACI %02x\n", i, memory[i + 1]);
                {
                    __uint16_t result = (__uint16_t)a + (__uint16_t)memory[i + 1] + (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i += 2;
                break;
            case 0xCF:
                printf("%02x   RST 1\n", i);
                {
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (0 << 8) | 8;
                }
                break;

            case 0xD0:
                printf("%02x   RNC\n", i);
                if (CF == 0){
                    i = (memory[sp + 1] << 8) | memory[sp];
                    sp += 2;
                } else {
                    i++;
                }
                break;
            case 0xD1:
                printf("%02x   POP D\n", i);
                d = memory[sp + 1];
                e = memory[sp];
                sp += 2;  
                i++;
                break;
            case 0xD2:
                printf("%02x   JNC %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (CF == 0){
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;

            case 0xD3:
                printf("%02x   OUT %02x\n", i, memory[i + 1]);
                
                //getchar();

                memory[memory[i + 1]] = a;

                i += 2;
                break;

            case 0xD4:
                printf("%02x   CNC %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (CF == 0){
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;
            case 0xD5:
                printf("%02x   PUSH D\n", i);
                memory[sp - 2] = e;
                memory[sp - 1] = d;
                sp -= 2;
                i++;
                break;
            case 0xD6:
                printf("%02x   SUI %02x\n", i, memory[i + 1]);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)memory[i + 1];
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i += 2;
                break;
            case 0xD7:
                printf("%02x   RST 2\n", i);
                {
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (0 << 8) | 16;
                }
                break;
            case 0xD8:
                printf("%02x   RC\n", i);
                if (CF){
                    i = (memory[sp + 1] << 8) | memory[sp];
                    sp += 2;
                } else {
                    i++;
                }
                break;
            case 0xD9:
                printf("%02x   RET\n", i);
                i = (memory[sp + 1] << 8) | memory[sp];
                sp += 2;
                break;
            case 0xDA:
                printf("%02x   JC %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (CF){
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;

            case 0xDB:
                printf("%02x   IN %02x\n", i, memory[i + 1]);
                a = memory[memory[i + 1]];

                //getchar();

                i += 2;
                break;

            case 0xDC:
                printf("%02x   CC %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (CF){
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;
            case 0xDD:
                printf("%02x   CALL %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                {
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                }
                break;
            case 0xDE:
                printf("%02x   SBI %02x\n", i, memory[i + 1]);
                {
                    __uint16_t result = (__uint16_t)a - (__uint16_t)memory[i + 1] - (__uint16_t)CF;
                    a = result & 0xff;
                    
                    CF = (result > 0xff);
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i += 2;
                break;
            case 0xDF:
                printf("%02x   RST 3\n", i);
                {
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (0 << 8) | 24;
                }
                break;

            case 0xE0:
                printf("%02x   RPO\n", i);
                if (PF == 0){
                    i = (memory[sp + 1] << 8) | memory[sp];
                    sp += 2;
                } else {
                    i++;
                }
                break;
            case 0xE1:
                printf("%02x   POP H\n", i);
                h = memory[sp + 1];
                l = memory[sp];
                sp += 2;  
                i++;
                break;
            case 0xE2:
                printf("%02x   JPO %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (PF == 0){
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;
            case 0xE3:
                printf("%02x   XTHL\n", i);
                {
                    __uint8_t temph = h;
                    __uint8_t templ = l;

                    l = memory[sp];
	                h = memory[sp + 1];

                    memory[sp] = templ;
	                memory[sp + 1] = temph;
                }
                i++;
                break;
            case 0xE4:
                printf("%02x   CPO %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (PF == 0){
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;
            case 0xE5:
                printf("%02x   PUSH H\n", i);
                memory[sp - 2] = l;
                memory[sp - 1] = h;
                sp -= 2;
                i++;
                break;
            case 0xE6:
                printf("%02x   ANI %02x\n", i, memory[i + 1]);
                {
                    __uint16_t result = (__uint16_t)a & (__uint16_t)memory[i + 1];
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = (result > 0x09);
                }
                i += 2;
                break;
            case 0xE7:
                printf("%02x   RST 4\n", i);
                {
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (0 << 8) | 32;
                }
                break;
            case 0xE8:
                printf("%02x   RPE\n", i);
                if (PF == 0){
                    i = (memory[sp + 1] << 8) | memory[sp];
                    sp += 2;
                } else {
                    i++;
                }
                break;
            case 0xE9:
                printf("%02x   PCHL\n", i);
                i = (h << 8) | l;
                break;
            case 0xEA:
                printf("%02x   JPE %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (PF){
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;
            case 0xEB:
                printf("%02x   XCHG\n", i);
                {
                    __uint8_t tempd = d;
                    __uint8_t tempe = e;

                    d = h;
                    e = l;

                    h = tempd;
                    l = tempe;
                }
                i++;
                break;
            case 0xEC:
                printf("%02x   CPE %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (PF){
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;
            case 0xED:
                printf("%02x   CALL %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                {
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                }
                break;
            case 0xEE:
                printf("%02x   XRI %02x\n", i, memory[i + 1]);
                {
                    __uint16_t result = (__uint16_t)a ^ (__uint16_t)memory[i + 1];
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i += 2;
                break;
            case 0xEF:
                printf("%02x   RST 5\n", i);
                {
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (0 << 8) | 40;
                }
                break;

            case 0xF0:
                printf("%02x   RP\n", i);
                if (SF == 0){
                    i = (memory[sp + 1] << 8) | memory[sp];
                    sp += 2;
                } else {
                    i++;
                }
                break;
            case 0xF1:
                printf("%02x   POP PSW\n", i);
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
                i++;
                break;
            case 0xF2:
                printf("%02x   JP %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (SF == 0){
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;

            case 0xF3:
                printf("%02x   DI\n", i);
                int_enable = 0;
                break;

            case 0xF4:
                printf("%02x   CP %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (SF == 0){
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;
            case 0xF5:
                printf("%02x   PUSH PSW\n", i);
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
                i++;
                break;
            case 0xF6:
                printf("%02x   ORI %02x\n", i, memory[i + 1]);
                {
                    __uint16_t result = (__uint16_t)a | (__uint16_t)memory[i + 1];
                    a = result;

                    CF = 0;
                    ZF = ((result & 0xff) == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result & 0xff, 8);
                    AF = 0;
                }
                i += 2;
                break;
            case 0xF7:
                printf("%02x   RST 6\n", i);
                {
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (0 << 8) | 48;
                }
                break;
            case 0xF8:
                printf("%02x   RM\n", i);
                if (SF){
                    i = (memory[sp + 1] << 8) | memory[sp];
                    sp += 2;
                } else {
                    i++;
                }
                break;
            case 0xF9:
                printf("%02x   SPHL\n", i);
                sp = (h << 8) | l;
                i++;
                break;
            case 0xFA:
                printf("%02x   JM %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (SF){
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;

            case 0xFB:
                printf("%02x   EI\n", i);
                int_enable = 1;
                break;

            case 0xFC:
                printf("%02x   CM %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                if (SF){
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                } else {
                    i += 3;
                }
                break;
            case 0xFD:
                printf("%02x   CALL %02x%02x\n", i, memory[i + 2], memory[i + 1]);
                {
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (memory[i + 2] << 8) | (memory[i + 1]);
                }
                break;
            case 0xFE:
                printf("%02x   CPI %02x\n", i, memory[i + 1]);
                {
                    __uint16_t result = a - memory[i + 1];
                    ZF = (result == 0);
                    SF = (0x80 == (result & 0x80));
                    PF = parity(result, 8);
                    CF = (a < memory[i + 1]);
                }
                i += 2;
                break;
            case 0xFF:
                printf("%02x   RST 7\n", i);
                {
                    __uint16_t ret = i + 3;
                    memory[sp - 1] = (ret >> 8) & 0xFF;
                    memory[sp - 2] = ret & 0xFF;
                    sp -= 2;
                    i = (0 << 8) | 56;
                }
                
                break;

            default:
                printf("%02x   Unimplemented instruction %02x\n", i, memory[i]);
                i++;
                break;
        }

        printf("pc: %02x sp: %02x a: %02x b: %02x c: %02x d: %02x e: %02x h: %02x l: %02x CF: %d PF: %d AF: %d ZF: %d SF: %d\n\n", i, sp, a, b, c, d, e, h, l, CF, PF, AF, ZF, SF);

        //getchar();
    }

    return 0;
}
