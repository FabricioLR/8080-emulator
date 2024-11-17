#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() { 
    FILE *file, *file1, *file2, *file3;
    int c;
    long length = 0;

    file = fopen("invaders.h", "rb");
    file1 = fopen("invaders.g", "rb");
    file2 = fopen("invaders.f", "rb");
    file3 = fopen("invaders.e", "rb");

    unsigned char code[15000];

    int i = 0;
    while (1){
        c = fgetc(file);

        if (c == EOF){
            break;
        }

        code[i] = c;
        i++;
    }

    while (1){
        c = fgetc(file1);
        if (c == EOF){
            break;
        }
        code[i] = c;
        i++;
    }

    while (1){
        c = fgetc(file2);
        if (c == EOF){
            break;
        }
        code[i] = c;
        i++;
    }

    while (1){
        c = fgetc(file3);
        if (c == EOF){
            break;
        }
        code[i] = c;
        i++;
    }

    fclose(file);
    fclose(file1);
    fclose(file2);
    fclose(file3);

    length = i;

    i = 0;
    while (i < length){
        //printf("%02x\n", code[i]);
        switch (code[i]){
            case 0x00:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x01:
                printf("%02x   LXI B, %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0x02:
                printf("%02x   STAX B\n", i);
                i++;
                break;
            case 0x03:
                printf("%02x   INX B\n", i);
                i++;
                break;
            case 0x04:
                printf("%02x   INR B\n", i);
                i++;
                break;
            case 0x05:
                printf("%02x   DCR B\n", i);
                i++;
                break;
            case 0x06:
                printf("%02x   MVI B, %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0x07:
                printf("%02x   RLC\n", i);
                i++;
                break;
            case 0x08:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x09:
                printf("%02x   DAD B\n", i);
                i++;
                break;
            case 0x0A:
                printf("%02x   LDAX B\n", i);
                i++;
                break;
            case 0x0B:
                printf("%02x   DCX B\n", i);
                i++;
                break;
            case 0x0C:
                printf("%02x   INR C\n", i);
                i++;
                break;
            case 0x0D:
                printf("%02x   DCR C\n", i);
                i++;
                break;
            case 0x0E:
                printf("%02x   MVI C, %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0x0F:
                printf("%02x   RRC\n", i);
                i++;
                break;

            case 0x10:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x11:
                printf("%02x   LXI D, %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0x12:
                printf("%02x   STAX D\n", i);
                i++;
                break;
            case 0x13:
                printf("%02x   INX D\n", i);
                i++;
                break;
            case 0x14:
                printf("%02x   INR D\n", i);
                i++;
                break;
            case 0x15:
                printf("%02x   DCR D\n", i);
                i++;
                break;
            case 0x16:
                printf("%02x   MVI D, %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0x17:
                printf("%02x   RAL\n", i);
                i++;
                break;
            case 0x18:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x19:
                printf("%02x   DAD D\n", i);
                i++;
                break;
            case 0x1A:
                printf("%02x   LDAX D\n", i);
                i++;
                break;
            case 0x1B:
                printf("%02x   DCX D\n", i);
                i++;
                break;
            case 0x1C:
                printf("%02x   INR E\n", i);
                i++;
                break;
            case 0x1D:
                printf("%02x   DCR E\n", i);
                i++;
                break;
            case 0x1E:
                printf("%02x   MVI E, %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0x1F:
                printf("%02x   RAR\n", i);
                i++;
                break;
            
            case 0x20:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x21:
                printf("%02x   LXI H, %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0x22:
                printf("%02x   SHLD %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0x23:
                printf("%02x   INX H\n", i);
                i++;
                break;
            case 0x24:
                printf("%02x   INR H\n", i);
                i++;
                break;
            case 0x25:
                printf("%02x   DCR H\n", i);
                i++;
                break;
            case 0x26:
                printf("%02x   MVI H, %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0x27:
                printf("%02x   DAA\n", i);
                i++;
                break;
            case 0x28:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x29:
                printf("%02x   DAD H\n", i);
                i++;
                break;
            case 0x2A:
                printf("%02x   LHLD %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0x2B:
                printf("%02x   DCX H\n", i);
                i++;
                break;
            case 0x2C:
                printf("%02x   INR L\n", i);
                i++;
                break;
            case 0x2D:
                printf("%02x   DCR L\n", i);
                i++;
                break;
            case 0x2E:
                printf("%02x   MVI L, %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0x2F:
                printf("%02x   CMA\n", i);
                i++;
                break;
            
            case 0x30:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x31:
                printf("%02x   LXI SP, %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0x32:
                printf("%02x   STA %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0x33:
                printf("%02x   INX SP\n", i);
                i++;
                break;
            case 0x34:
                printf("%02x   INR M\n", i);
                i++;
                break;
            case 0x35:
                printf("%02x   DCR M\n", i);
                i++;
                break;
            case 0x36:
                printf("%02x   MVI M, %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0x37:
                printf("%02x   STC\n", i);
                i++;
                break;
            case 0x38:
                printf("%02x   NOOP\n", i);
                i++;
                break;
            case 0x39:
                printf("%02x   DAD SP\n", i);
                i++;
                break;
            case 0x3A:
                printf("%02x   LDA %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0x3B:
                printf("%02x   DCX SP\n", i);
                i++;
                break;
            case 0x3C:
                printf("%02x   INR A\n", i);
                i++;
                break;
            case 0x3D:
                printf("%02x   DCR A\n", i);
                i++;
                break;
            case 0x3E:
                printf("%02x   MVI A, %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0x3F:
                printf("%02x   CMC\n", i);
                i++;
                break;
            
            case 0x40:
                printf("%02x   MOV B, B\n", i);
                i++;
                break;
            case 0x41:
                printf("%02x   MOV B, C\n", i);
                i++;
                break;
            case 0x42:
                printf("%02x   MOV B, D\n", i);
                i++;
                break;
            case 0x43:
                printf("%02x   MOV B, E\n", i);
                i++;
                break;
            case 0x44:
                printf("%02x   MOV B, H\n", i);
                i++;
                break;
            case 0x45:
                printf("%02x   MOV B, L\n", i);
                i++;
                break;
            case 0x46:
                printf("%02x   MOV B, M\n", i);
                i++;
                break;
            case 0x47:
                printf("%02x   MOV B, A\n", i);
                i++;
                break;
            case 0x48:
                printf("%02x   MOV C, B\n", i);
                i++;
                break;
            case 0x49:
                printf("%02x   MOV C, C\n", i);
                i++;
                break;
            case 0x4A:
                printf("%02x   MOC C, D\n", i);
                i++;
                break;
            case 0x4B:
                printf("%02x   MOV C, E\n", i);
                i++;
                break;
            case 0x4C:
                printf("%02x   MOV C, H\n", i);
                i++;
                break;
            case 0x4D:
                printf("%02x   MOV C, L\n", i);
                i++;
                break;
            case 0x4E:
                printf("%02x   MOV C, M\n", i);
                i++;
                break;
            case 0x4F:
                printf("%02x   MOV C, A\n", i);
                i++;
                break;
            
            case 0x50:
                printf("%02x   MOV D, B\n", i);
                i++;
                break;
            case 0x51:
                printf("%02x   MOV D, C\n", i);
                i++;
                break;
            case 0x52:
                printf("%02x   MOV D, D\n", i);
                i++;
                break;
            case 0x53:
                printf("%02x   MOV D, E\n", i);
                i++;
                break;
            case 0x54:
                printf("%02x   MOV D, H\n", i);
                i++;
                break;
            case 0x55:
                printf("%02x   MOV D, L\n", i);
                i++;
                break;
            case 0x56:
                printf("%02x   MOV D, M\n", i);
                i++;
                break;
            case 0x57:
                printf("%02x   MOV D, A\n", i);
                i++;
                break;
            case 0x58:
                printf("%02x   MOV E, B\n", i);
                i++;
                break;
            case 0x59:
                printf("%02x   MOV E, C\n", i);
                i++;
                break;
            case 0x5A:
                printf("%02x   MOC E, D\n", i);
                i++;
                break;
            case 0x5B:
                printf("%02x   MOV E, E\n", i);
                i++;
                break;
            case 0x5C:
                printf("%02x   MOV E, H\n", i);
                i++;
                break;
            case 0x5D:
                printf("%02x   MOV E, L\n", i);
                i++;
                break;
            case 0x5E:
                printf("%02x   MOV E, M\n", i);
                i++;
                break;
            case 0x5F:
                printf("%02x   MOV E, A\n", i);
                i++;
                break;
            
            case 0x60:
                printf("%02x   MOV H, B\n", i);
                i++;
                break;
            case 0x61:
                printf("%02x   MOV H, C\n", i);
                i++;
                break;
            case 0x62:
                printf("%02x   MOV H, D\n", i);
                i++;
                break;
            case 0x63:
                printf("%02x   MOV H, E\n", i);
                i++;
                break;
            case 0x64:
                printf("%02x   MOV H, H\n", i);
                i++;
                break;
            case 0x65:
                printf("%02x   MOV H, L\n", i);
                i++;
                break;
            case 0x66:
                printf("%02x   MOV H, M\n", i);
                i++;
                break;
            case 0x67:
                printf("%02x   MOV H, A\n", i);
                i++;
                break;
            case 0x68:
                printf("%02x   MOV L, B\n", i);
                i++;
                break;
            case 0x69:
                printf("%02x   MOV L, C\n", i);
                i++;
                break;
            case 0x6A:
                printf("%02x   MOC L, D\n", i);
                i++;
                break;
            case 0x6B:
                printf("%02x   MOV L, E\n", i);
                i++;
                break;
            case 0x6C:
                printf("%02x   MOV L, H\n", i);
                i++;
                break;
            case 0x6D:
                printf("%02x   MOV L, L\n", i);
                i++;
                break;
            case 0x6E:
                printf("%02x   MOV L, M\n", i);
                i++;
                break;
            case 0x6F:
                printf("%02x   MOV L, A\n", i);
                i++;
                break;
            
            case 0x70:
                printf("%02x   MOV M, B\n", i);
                i++;
                break;
            case 0x71:
                printf("%02x   MOV M, C\n", i);
                i++;
                break;
            case 0x72:
                printf("%02x   MOV M, D\n", i);
                i++;
                break;
            case 0x73:
                printf("%02x   MOV M, E\n", i);
                i++;
                break;
            case 0x74:
                printf("%02x   MOV M, H\n", i);
                i++;
                break;
            case 0x75:
                printf("%02x   MOV M, L\n", i);
                i++;
                break;
            case 0x76:
                printf("%02x   HALT\n", i);
                i++;
                break;
            case 0x77:
                printf("%02x   MOV M, A\n", i);
                i++;
                break;
            case 0x78:
                printf("%02x   MOV A, B\n", i);
                i++;
                break;
            case 0x79:
                printf("%02x   MOV A, C\n", i);
                i++;
                break;
            case 0x7A:
                printf("%02x   MOC A, D\n", i);
                i++;
                break;
            case 0x7B:
                printf("%02x   MOV A, E\n", i);
                i++;
                break;
            case 0x7C:
                printf("%02x   MOV A, H\n", i);
                i++;
                break;
            case 0x7D:
                printf("%02x   MOV A, L\n", i);
                i++;
                break;
            case 0x7E:
                printf("%02x   MOV A, M\n", i);
                i++;
                break;
            case 0x7F:
                printf("%02x   MOV A, A\n", i);
                i++;
                break;

            case 0x80:
                printf("%02x   ADD B\n", i);
                i++;
                break;
            case 0x81:
                printf("%02x   ADD C\n", i);
                i++;
                break;
            case 0x82:
                printf("%02x   ADD D\n", i);
                i++;
                break;
            case 0x83:
                printf("%02x   ADD E\n", i);
                i++;
                break;
            case 0x84:
                printf("%02x   ADD H\n", i);
                i++;
                break;
            case 0x85:
                printf("%02x   ADD L\n", i);
                i++;
                break;
            case 0x86:
                printf("%02x   ADD M\n", i);
                i++;
                break;
            case 0x87:
                printf("%02x   ADD A\n", i);
                i++;
                break;
            case 0x88:
                printf("%02x   ADC B\n", i);
                i++;
                break;
            case 0x89:
                printf("%02x   ADC C\n", i);
                i++;
                break;
            case 0x8A:
                printf("%02x   ADC D\n", i);
                i++;
                break;
            case 0x8B:
                printf("%02x   ADC E\n", i);
                i++;
                break;
            case 0x8C:
                printf("%02x   ADC H\n", i);
                i++;
                break;
            case 0x8D:
                printf("%02x   ADC L\n", i);
                i++;
                break;
            case 0x8E:
                printf("%02x   ADC M\n", i);
                i++;
                break;
            case 0x8F:
                printf("%02x   ADC A\n", i);
                i++;
                break;
            
            case 0x90:
                printf("%02x   SUB B\n", i);
                i++;
                break;
            case 0x91:
                printf("%02x   SUB C\n", i);
                i++;
                break;
            case 0x92:
                printf("%02x   SUB D\n", i);
                i++;
                break;
            case 0x93:
                printf("%02x   SUB E\n", i);
                i++;
                break;
            case 0x94:
                printf("%02x   SUB H\n", i);
                i++;
                break;
            case 0x95:
                printf("%02x   SUB L\n", i);
                i++;
                break;
            case 0x96:
                printf("%02x   SUB M\n", i);
                i++;
                break;
            case 0x97:
                printf("%02x   SUB A\n", i);
                i++;
                break;
            case 0x98:
                printf("%02x   SBB B\n", i);
                i++;
                break;
            case 0x99:
                printf("%02x   SBB C\n", i);
                i++;
                break;
            case 0x9A:
                printf("%02x   SBB D\n", i);
                i++;
                break;
            case 0x9B:
                printf("%02x   SBB E\n", i);
                i++;
                break;
            case 0x9C:
                printf("%02x   SBB H\n", i);
                i++;
                break;
            case 0x9D:
                printf("%02x   SBB L\n", i);
                i++;
                break;
            case 0x9E:
                printf("%02x   SBB M\n", i);
                i++;
                break;
            case 0x9F:
                printf("%02x   SBB A\n", i);
                i++;
                break;

            case 0xA0:
                printf("%02x   ANA B\n", i);
                i++;
                break;
            case 0xA1:
                printf("%02x   ANA C\n", i);
                i++;
                break;
            case 0xA2:
                printf("%02x   ANA D\n", i);
                i++;
                break;
            case 0xA3:
                printf("%02x   ANA E\n", i);
                i++;
                break;
            case 0xA4:
                printf("%02x   ANA H\n", i);
                i++;
                break;
            case 0xA5:
                printf("%02x   ANA L\n", i);
                i++;
                break;
            case 0xA6:
                printf("%02x   ANA M\n", i);
                i++;
                break;
            case 0xA7:
                printf("%02x   ANA A\n", i);
                i++;
                break;
            case 0xA8:
                printf("%02x   XRA B\n", i);
                i++;
                break;
            case 0xA9:
                printf("%02x   XRA C\n", i);
                i++;
                break;
            case 0xAA:
                printf("%02x   XRA D\n", i);
                i++;
                break;
            case 0xAB:
                printf("%02x   XRA E\n", i);
                i++;
                break;
            case 0xAC:
                printf("%02x   XRA H\n", i);
                i++;
                break;
            case 0xAD:
                printf("%02x   XRA L\n", i);
                i++;
                break;
            case 0xAE:
                printf("%02x   XRA M\n", i);
                i++;
                break;
            case 0xAF:
                printf("%02x   XRA A\n", i);
                i++;
                break;

            case 0xB0:
                printf("%02x   ORA B\n", i);
                i++;
                break;
            case 0xB1:
                printf("%02x   ORA C\n", i);
                i++;
                break;
            case 0xB2:
                printf("%02x   ORA D\n", i);
                i++;
                break;
            case 0xB3:
                printf("%02x   ORA E\n", i);
                i++;
                break;
            case 0xB4:
                printf("%02x   ORA H\n", i);
                i++;
                break;
            case 0xB5:
                printf("%02x   ORA L\n", i);
                i++;
                break;
            case 0xB6:
                printf("%02x   ORA M\n", i);
                i++;
                break;
            case 0xB7:
                printf("%02x   ORA A\n", i);
                i++;
                break;
            case 0xB8:
                printf("%02x   CMP B\n", i);
                i++;
                break;
            case 0xB9:
                printf("%02x   CMP C\n", i);
                i++;
                break;
            case 0xBA:
                printf("%02x   CMP D\n", i);
                i++;
                break;
            case 0xBB:
                printf("%02x   CMP E\n", i);
                i++;
                break;
            case 0xBC:
                printf("%02x   CMP H\n", i);
                i++;
                break;
            case 0xBD:
                printf("%02x   CMP L\n", i);
                i++;
                break;
            case 0xBE:
                printf("%02x   CMP M\n", i);
                i++;
                break;
            case 0xBF:
                printf("%02x   CMP A\n", i);
                i++;
                break;

            case 0xC0:
                printf("%02x   RNZ\n", i);
                i++;
                break;
            case 0xC1:
                printf("%02x   POP B\n", i);
                i++;
                break;
            case 0xC2:
                printf("%02x   JNZ %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xC3:
                printf("%02x   JMP %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xC4:
                printf("%02x   CNZ %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xC5:
                printf("%02x   PUSH B\n", i);
                i++;
                break;
            case 0xC6:
                printf("%02x   ADI %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0xC7:
                printf("%02x   RST 0\n", i);
                i++;
                break;
            case 0xC8:
                printf("%02x   RZ\n", i);
                i++;
                break;
            case 0xC9:
                printf("%02x   RET\n", i);
                i++;
                break;
            case 0xCA:
                printf("%02x   JZ %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xCB:
                printf("%02x   JMP %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xCC:
                printf("%02x   CZ %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xCD:
                printf("%02x   CALL %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xCE:
                printf("%02x   ACI %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0xCF:
                printf("%02x   RST 1\n", i);
                i++;
                break;

            case 0xD0:
                printf("%02x   RNC\n", i);
                i++;
                break;
            case 0xD1:
                printf("%02x   POP D\n", i);
                i++;
                break;
            case 0xD2:
                printf("%02x   JNC %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xD3:
                printf("%02x   OUT %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0xD4:
                printf("%02x   CNC %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xD5:
                printf("%02x   PUSH D\n", i);
                i++;
                break;
            case 0xD6:
                printf("%02x   SUI %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0xD7:
                printf("%02x   RST 2\n", i);
                i++;
                break;
            case 0xD8:
                printf("%02x   RC\n", i);
                i++;
                break;
            case 0xD9:
                printf("%02x   RET\n", i);
                i++;
                break;
            case 0xDA:
                printf("%02x   JC %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xDB:
                printf("%02x   IN %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0xDC:
                printf("%02x   CC %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xDD:
                printf("%02x   CALL %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xDE:
                printf("%02x   SBI %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0xDF:
                printf("%02x   RST 3\n", i);
                i++;
                break;

            case 0xE0:
                printf("%02x   RPO\n", i);
                i++;
                break;
            case 0xE1:
                printf("%02x   POP H\n", i);
                i++;
                break;
            case 0xE2:
                printf("%02x   JPO %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xE3:
                printf("%02x   XTHL\n", i);
                i++;
                break;
            case 0xE4:
                printf("%02x   CPO %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xE5:
                printf("%02x   PUSH H\n", i);
                i++;
                break;
            case 0xE6:
                printf("%02x   ANI %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0xE7:
                printf("%02x   RST 4\n", i);
                i++;
                break;
            case 0xE8:
                printf("%02x   RPE\n", i);
                i++;
                break;
            case 0xE9:
                printf("%02x   PCHL\n", i);
                i++;
                break;
            case 0xEA:
                printf("%02x   JPE %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xEB:
                printf("%02x   XCHG\n", i);
                i++;
                break;
            case 0xEC:
                printf("%02x   CPE %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xED:
                printf("%02x   CALL %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xEE:
                printf("%02x   XRI %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0xEF:
                printf("%02x   RST 5\n", i);
                i++;
                break;

            case 0xF0:
                printf("%02x   RP\n", i);
                i++;
                break;
            case 0xF1:
                printf("%02x   POP PSW\n", i);
                i++;
                break;
            case 0xF2:
                printf("%02x   JP %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xF3:
                printf("%02x   DI\n", i);
                i++;
                break;
            case 0xF4:
                printf("%02x   CP %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xF5:
                printf("%02x   PUSH PSW\n", i);
                i++;
                break;
            case 0xF6:
                printf("%02x   ORI %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0xF7:
                printf("%02x   RST 6\n", i);
                i++;
                break;
            case 0xF8:
                printf("%02x   RM\n", i);
                i++;
                break;
            case 0xF9:
                printf("%02x   SPHL\n", i);
                i++;
                break;
            case 0xFA:
                printf("%02x   JM %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xFB:
                printf("%02x   EI\n", i);
                i++;
                break;
            case 0xFC:
                printf("%02x   CM %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xFD:
                printf("%02x   CALL %02x%02x\n", i, code[i + 2], code[i + 1]);
                i += 3;
                break;
            case 0xFE:
                printf("%02x   CPI %02x\n", i, code[i + 1]);
                i += 2;
                break;
            case 0xFF:
                printf("%02x   RST 7\n", i);
                i++;
                break;

            default:
                printf("%02x   \n", i);
                i++;
                break;
        }
    }

    return 0;
}
