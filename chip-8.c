//
//  chip-8.c
//  
//
//  Created by Adenike Adeyemi on 5/17/17.
//
//

#include <stdio.h>
#include <stdlib.h>

/*
 Opcodes
 ===================================================
 NNN     address
 NN      8-bit constant
 N       4-bit constant
 X and Y 4-bit register id
 PC      Program counter
 I       16-bit register (For memory address)
 
 #  Opcode  Type                    C Psudo Code
 1  0NNN    Call                    BIOS call not needed
 2  00E0    Display                 disp_clear()
 3  00EE    Flow                    return;
 4  1NNN    Flow                    goto NNN;
 5  2NNN    Flow                    *(0xNNN)()
 6  3XNN    Condition               if(Vx==NN)
 7  4XNN    Condition               if(Vx!=NN)
 8  5XY0    Condition               if(Vx==Vy)
 9  6XNN    Constant                Vx=NN
 10 7XNN    Constant                Vx+=NN
 11 8XY0    Assign                  Vx=Vy
 12 8XY1    Bit Operation           Vx=Vx|Vy
 13 8XY2    Bit Operation           Vx=Vx&Vy
 14 8XY3    Bit Operation           Vx=Vx^Vy
 15 8XY4    Math                    Vx+=Vy
 16 8XY5    Math                    Vx-=Vy
 17 8XY6    Bit Operation           Vx>>1
 18 8XY7    Math                    Vx=Vy-Vx
 19 8XYE    Bit Operation           Vx<<1
 20 9XY0    Condition               if(Vx!=Vy)
 21 ANNN    Memory                  I = NNN
 22 BNNN    Flow                    PC = V0+NNN
 23 CXNN    Rand                    Vx=rand()&NN
 24 DXYN    Display                 draw(Vx, Vy, N)
 25 EX9E    Key Operation           if(key() == Vx)
 26 EXA1    Key Operation           if(key() != Vx)
 27 FX07    Timer                   delay_timer(Vx)
 28 FX0A    Key Operation           Vx=getKey()
 29 FX15    Timer                   delay_timer(Vx)
 30 FX18    Sound                   sound_timer(Vx)
 31 FX1E    Memory                  I+=Vx
 32 FX29    Memory                  I=sprite_addr[Vx]
 33 FX33    Binary-Coded Decimal    set_BCD(Vx)
                                    *(I+0) = BCD(3); hundredth
                                    *(I+1) = BCD(2); tenths
                                    *(I+2) = BCD(1); ones
 34 FX55    Memory                  reg_dump(Vx,&I);
 35 FX65    Memory                  reg_load(Vx,&I);
 
 
 Memory Map
 ================================================================
 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
 0x050-0x0A0 - Used for the built 4x5 pixel font set (0-F)
 0x200-0xFFF - Program Rom and work RAM
 
 */
unsigned char chip8_fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};



unsigned short opcode;

unsigned char memory[4096]; //4K memory

unsigned char V[16]; //VF or V[15] is for flags do not touch
unsigned short I;    //stores pointers do not touch
unsigned short pc;   //stores next instuction do not touch

unsigned char gfx[64 * 32]; //64x32 display graphics

unsigned char delay_timer;  //counts down to 0
unsigned char sound_timer;  //counts down to 0 and sounds off

unsigned short stack[16];
unsigned short sp;

unsigned char key[16];

void chip8_Init(){

    
    I = 0;      //clear Index Register
    sp = 0;     //clear stack pointer
    opcode = 0; //clear opcodes
    pc = 0x200; //starting point in mem
    
    //clear memory
    for(int i = 0; i<4096; i++){
        memory[i] = 0;
    }
    
    //clear registers
    for(int i = 0; i < 16; i++){
        V[i] = 0;
    }
    
    //clear stack
    for (int i = 0; i < 16; i++) {
        stack[i] = 0;
    }
    
    //clear display
    for (int i = 0; i<(64*32); i++) {
        gfx[i] = 0;
    }
    
   
    //set fontset at memory 0x50 or 80
    for(int i = 0; i < 80; i++){
        memory[i+80] = chip8_fontset[i];
    }

    
    delay_timer = 0;
    sound_timer = 0;
    
    
}

void chip8_load(char *filename){
    //open file
    FILE *fp;
    unsigned char buffer[3584]; //space for loaded program
    size_t bufferSize;  //size of loaded program
    
    fp = fopen(filename, rb); //open file and read it in binary
    if(fp == NULL){
        printf("Program %s does not exists", filename); //if file is no valid exit program
        exit(0);
    }
    
    //load data into buffer and get bufferSize
    bufferSize = fread(buffer, 1, 3583, fp);
    
    
    //load program TODO: get buffersize and buffer
    for(int i = 0; i < bufferSize; i++){
        memory[i+512] = buffer[i];
    }

}

void chip8_emulateCycle(){
    //Fetch opcode
    opcode = (memory[pc] << 8) | memory[pc+1];
    pc+=2;
    
    //Decode opcode
    switch(opcode&0xF000){
    
        case 0x0000:
            switch(opcode&0x0FFF){
                case 0x0E0://00E0 clear display
                    
                    for (int i = 0; i<(64*32); i++) {
                        gfx[i] = 0;
                    }
                    break;
                case 0x0EE:
                    //TODO: get return pc from stack
                    pc = stack[sp]; //get pc from stack
                    sp--;           //move stack pointer down
                    break;
                default:// 0x0NNN not needed
                    printf("Opcode [0x000]: Invalid Opcode 0x%x", opcode);
                    break;
            }
            break;
        case 0x1000:
            pc = opcode & 0x0FFF;   //1NNN Jump to NNN
            break;
        case 0x2000:
            stack[sp]=pc;           //push program counter into stack
            sp++;                   //increment stack pointer
            
            pc = opcode & 0x0FFF;   //2NNN jump to function at NNN
            break;
        case 0x3000:
            //3XNN if VX == NN skip
            if(V[(opcode&0x0F00)>>8] == opcode&0x00FF)
                pc+=2;
            
            break;
        case 0x4000:
            //4XNN if VX != NN skip
            if(V[(opcode&0x0F00)>>8] != opcode&0x00FF)
                pc+=2;
            
            break;
        case 0x5000:
            //5XNN if VX == VY skip
            if(V[(opcode&0x0F00)>>8] == V[(opcode&0x00F0)>>4])
                pc+=2;
            
            break;
        case 0x6000:
            //6XNN Vx = NN
            V[opcode&0x0F00>>8] = opcode&0x00FF;
            
            break;
        case 0x7000:
            //7XNN Vx += NN
            V[opcode&0x0F00>>8] += opcode&0x00FF;
            break;
        case 0x8000:
            switch(opcode&0x000F){
                case 0x0000: //8XY0 Vx = Vy
                    V[(opcode&0x0F00)>>8] = V[(opcode&0x00F0)>>4];
                    break;
                case 0x0001: //8XY1 Vx = Vx | Vy
                    V[(opcode&0x0F00)>>8] = V[(opcode&0x0F00)>>8] | V[(opcode&0x00F0)>>4];
                    break;
                case 0x0002: //8XY2 Vx = Vx & Vy
                    V[(opcode&0x0F00)>>8] = V[(opcode&0x0F00)>>8] & V[(opcode&0x00F0)>>4];
                    break;
                case 0x0003: //8XY3 Vx = Vx ^ Vy
                    V[(opcode&0x0F00)>>8] = V[(opcode&0x0F00)>>8] ^ V[(opcode&0x00F0)>>4];
                    break;
                case 0x0004: //8XY4 Vx += Vy Vx + Vy
                    if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
                        V[15] = 1;
                    else
                        V[15] = 0;
                    
                    V[(opcode&0x0F00)>>8] += V[(opcode&0x00F0)>>4];
                    
                    break;
                case 0x0005: //8XY5 Vx -= Vy Vx - Vy
                    if(V[(opcode & 0x00F0) >> 4] > (V[(opcode & 0x0F00) >> 8]))
                        V[15] = 0;
                    else
                        V[15] = 1;
                    V[(opcode&0x0F00)>>8] -= V[(opcode&0x00F0)>>4];
                    break;
                case 0x0006: //8XY6 Vx>>1
                    V[15] = V[(opcode&0x0F00)>>8] & 0x0001;
                    V[(opcode&0x0F00)>>8] = V[(opcode&0x0F00)>>8] >> 1;
                    break;
                case 0x0007: //8XY7 Vx = Vy - Vx
                    if(V[(opcode & 0x00F0) >> 4] < (V[(opcode & 0x0F00) >> 8]))
                        V[15] = 0;
                    else
                        V[15] = 1;
                    V[(opcode&0x0F00)>>8] = V[(opcode&0x00F0)>>4] - V[(opcode&0x0F00)>>8];
                    break;
                case 0x000E: //8XYE Vx<<1
                    V[15] = (V[(opcode&0x0F00)>>8] & 0x8000) >> 15;

                    V[(opcode&0x0F00)>>8] = V[(opcode&0x0F00)>>8] << 1;
                    break;
            }
            break;
        case 0x9000: //9XY0 if VX != VY skip
            if(V[(opcode&0x0F00)>>8] != V[(opcode&0x00F0)>>4])
                pc+=2;
            break;
        case 0xA000: //ANNN I = NNN
            I = opcode & 0x0FFF;
            break;
        case 0xB000: //BNNN pc = V0 + NNN
            pc = V[0] + (opcode&0x0FFF);
            break;
        case 0xC000: //CXNN VX = rand()&NN
            V[(opcode&0x0F00)>>8] = (rand()%256)&(opcode&0x00FF);
            break;
        case 0xD000: //DXYN draw(Vx, Vy, N)
            //TODO draw function and VF
            break;
        case 0xE000:
            switch(opcode&0x00FF){
                case 0x9E: //EX9E
                    //TODO key() == VX
                    break;
                case 0xA1: //EXA1
                    //TODO key() != VX
                    break;
            }
            break;
        case 0xF000:
            switch(opcode&0x00FF){
                case 0x07: //FX07 Vx = getDelay Timer
                    V[(opcode&0x0F00)>>8] = delay_timer;
                    break;
                case 0x0A: //FX0A Vx = get_key();
                    //TODO Vx = getkey()
                    break;
                case 0x15: //FX15 delay_timer = Vx
                    delay_timer = V[(opcode&0x0F00)>>8];
                    break;
                case 0x18: //FX18 sound_timer = Vx
                    sound_timer = V[(opcode&0x0F00)>>8];
                    break;
                case 0x1E: //FX1E I += Vx
                    
                    I += V[(opcode&0x0F00)>>8];
//                    if(I > 0xFFF)
//                        V[15] = 1;
//                    else
//                        V[15] = 0;
                    break;
                case 0x29:
                    //TODO sprite_address(VX)
                    break;
                case 0x33:
                case 0x07: //FX07 Vx = getDelay Timer
                    V[(opcode&0x0F00)>>8] = delay_timer;
                    break;
                case 0x0A: //FX0A Vx = get_key();
                    //TODO Vx = getkey()
                    break;
                case 0x15: //FX15 delay_timer = Vx
                    delay_timer = V[(opcode&0x0F00)>>8];
                    break;
                case 0x18: //FX18 sound_timer = Vx
                    sound_timer = V[(opcode&0x0F00)>>8];
                    break;
                case 0x1E: //FX1E I += Vx
                    I += V[(opcode&0x0F00)>>8];
                    break;
                case 0x29:
                    //TODO sprite_address(VX)
                    break;
                case 0x33: //FX33 Binary coded decimal
                    memory[I] = V[(opcode&0x0F00)>>8] / 100;
                    memory[I+1] = (V[(opcode&0x0F00)>>8] % 100) / 10;
                    memory[I+2] = (V[(opcode&0x0F00)>>8] % 100) % 10;
                    break;
                case 0x55: //FX55 Stores V0 to VX inclusive in memory I
                    for(int i = 0; i <= (opcode&0x0F00>>8); i++)
                        memory[I+i] = V[i];
                    break;
                case 0x65: //FX65 Loads V0 to VX inclusive from memory I
                    for(int i = 0; i <= (opcode&0x0F00>>8); i++)
                        V[i] = memory[I+i];
                    break;
                    
                                   

            }
        default:
             printf("Unknown opcode: 0x%X\n", opcode);
            break;
    
    }//end of decoding opcodes
    
    
    //update timers
    if(delay_timer > 0)
        delay_timer--;
    
    if(sound_timer > 0){
        if(sound_timer == 1)
            printf("BEEP!\n");
        
        sound_timer--;
    }
}


int main(){
    
    
}
