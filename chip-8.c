//
//  chip-8.c
//  
//
//  Created by Adenike Adeyemi on 5/17/17.
//
//

#include <stdio.h>

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
 1  0NNN    Call
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



int main(){
    
    
}
