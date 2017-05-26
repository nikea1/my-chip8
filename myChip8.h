//
//  myChip8.h
//  
//
//  Created by Adenike Adeyemi on 5/17/17.
//
//

#ifndef myChip8_h
#define myChip8_h
//TODO sprite address opcodes

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


void chip8_Debug_Display_Memory();          //displays all values in memory
void chip8_Debug_Display_All_Register();    //displays values in all registers
void chip8_Debug_Display_Register(int i);   //displays value for Vi
unsigned short chip8_Debug_Get_Opcode();    //displays current opcode
void chip8_Debug_Display_Stack();           //displays stack pointer and stack

void chip8_Init();                          //initializes chip8
void chip8_load(char *filename);            //loads file
void chip8_emulateCycle();                  //run one emulation cycle

typedef struct Chip8{
    
    unsigned short opcode;
    unsigned char memory[4096]; //4K memory
    unsigned char V[16];        //VF or V[15] is for flags do not touch
    unsigned short I;           //stores pointers do not touch
    unsigned short pc;          //stores next instuction do not touch
    unsigned char gfx[64 * 32]; //64x32 display graphics
    unsigned char delay_timer;  //counts down to 0
    unsigned char sound_timer;  //counts down to 0 and sounds off
    unsigned short stack[16];   //stack
    unsigned short sp;          //stack pointer
    unsigned char key[16];      //keypad
    
    
    
}Chip8;

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


#endif /* myChip8_h */
