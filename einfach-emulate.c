
#include <stdio.h>
#include <stdint.h>
#include <windows.h>

const float CLOCK_MULTIPLIER = 0.1;

//OPCODES
//const uint8_t FREEINSTRUCTION = 0x0;
const uint8_t ADD = 0x1;
const uint8_t ADDI = 0x2;
#define SUB 0x3;
const uint8_t SUBI = 0x4;
const uint8_t MOV = 0x5;
const uint8_t MOVI = 0x6;
const uint8_t SLLI = 0x7;
const uint8_t SLRI = 0x8;
const uint8_t J = 0xA;

//SPECIAL REIGSTERS
uint8_t pc = 0x0;
uint8_t inst_reg = 0x0;
uint8_t acc = 0x0;
uint8_t sp = 0x0;
uint8_t flag = 0x0;

//GP REGISTERS
uint8_t r0 = 0x0;
uint8_t r1 = 0x0;
uint8_t r2 = 0x0;
uint8_t r3 = 0x0;

//MEMORY
uint8_t mar = 0x0;
uint16_t mdr = 0x0;

uint16_t ram[256];

//TODO: EMULATE BUSES(ADDRESS, DATA, CONTROL) | FIX INDEXING ON ARRAY VS MAR 

void movi(uint8_t constant, uint8_t d)
{
    switch(d)
    {
        case 0:
            acc = constant;
            r0 = acc;
            break;
        case 1:
            acc = constant;
            r1 = acc;
            break;
        case 2:
            acc = constant;
            r2 = acc;
            break;
        case 3:
            acc = constant;
            r3 = acc;
            break;
    }
}

void subi(uint8_t constant, uint8_t d)
{
    switch(d)
    {
        case 0:
            acc = r0;
            acc = acc - constant;
            r0 = acc;
            break;
        case 1:
            acc = r1;
            acc = acc - constant;
            r1 = acc;
            break;
        case 2:
            acc = r2;
            acc = acc - constant;
            r2 = acc;
            break;
        case 3:
            acc = r3;
            acc = acc - constant;   
            r3 = acc;
            break;
    }
}

void addi(uint8_t constant, uint8_t d)
{
    switch(d)
    {
        case 0:
            acc = r0;
            acc = acc + constant;
            r0 = acc;
            break;
        case 1:
            acc = r1;
            acc = acc + constant;
            r1 = acc;
            break;
        case 2:
            acc = r2;
            acc = acc + constant;
            r2 = acc;
            break;
        case 3:
            acc = r3;
            acc = acc + constant;   
            r3 = acc;
            break;
    }
}

void slli(uint8_t constant, uint8_t d)
{
    switch(d)
    {
        case 0:
            acc = r0;
            acc = acc << constant;
            r0 = acc;
            break;
        case 1:
            acc = r1;
            acc = acc << constant;
            r1 = acc;
            break;
        case 2:
            acc = r2;
            acc = acc << constant;
            r2 = acc;
            break;
        case 3:
            acc = r3;
            acc = acc << constant;   
            r3 = acc;
            break;
    }
}

void slri(uint8_t constant, uint8_t d)
{
    switch(d)
    {
        case 0:
            acc = r0;
            acc = acc >> constant;
            r0 = acc;
            break;
        case 1:
            acc = r1;
            acc = acc >> constant;
            r1 = acc;
            break;
        case 2:
            acc = r2;
            acc = acc >> constant;
            r2 = acc;
            break;
        case 3:
            acc = r3;
            acc = acc >> constant;   
            r3 = acc;
            break;
    }
}

void j(uint8_t location)
{
    pc = location - 2;
}

void decodeInst(uint8_t opcode)
{
    uint8_t constant, d, s, t;
    d = ((inst_reg << 3) ^ (mdr >> 8));

    if((opcode ^ MOVI) == 0x0) { constant = mdr; printf("ITS A MOVI on $d: %d, with value: %d\n", d, constant); movi(constant, d); }
    else if((opcode ^ ADD) == 0x0){ printf("ITS AN ADD on $d: %d, $s: %d, $t: %d\n", d, s, t); }
    else if((opcode ^ ADDI) == 0x0) { constant = mdr; printf("ITS AN ADDI on $d: %d, with value: %d\n", d, constant); addi(constant, d);}
    else if((opcode ^ SUBI) == 0x0) {constant = mdr; printf("ITS A SUBI on $d: %d, with value: %d\n", d, constant); subi(constant, d);}
    else if((opcode ^ J) == 0x0){constant = mdr; printf("ITS A JMP TO ADDRESS: 0x%X\n", constant); j(constant);}
    else if((opcode ^ SLLI) == 0x0){constant = mdr; printf("ITS A SLLI on $d: %d, with value:%d\n", d, constant); slli(constant, d);}
    else if((opcode ^ SLRI) == 0x0){constant = mdr; printf("ITS A SLRI on $d: %d, with value:%d\n", d, constant); slli(constant, d);}

}

void showRegisters()
{
    printf("R0: 0x%X(%d)", r0, r0);
    printf("\t\tR1: 0x%X(%d)", r1, r1);
    printf("\t\tR2: 0x%X(%d)", r2, r2);
    printf("\t\tR3: 0x%X(%d)", r3, r3);
    printf("\n");
}

void showPC()
{
    printf("0x%X\n", pc);
}

void incrementPC()
{
    pc = pc + 0x2;
}

void nextCycle()
{
    Sleep(1000 * CLOCK_MULTIPLIER);
}

void step()
{
    if(ram[mar] != NULL)
    {
        //showPC();
        mar = pc;
        mdr = ram[mar];
        nextCycle();
        inst_reg = mdr >> 0xB;
        nextCycle();
        decodeInst(inst_reg);
        nextCycle();
        incrementPC();
        showRegisters();
        
        step();

    }
}

int main()
{
    ram[0] = 0x11FF;
    ram[2] = 0x3100;
    ram[4] = 0x5000;
    printf("EINFACH EMULATOR\n");
    step();
    return 0;
}