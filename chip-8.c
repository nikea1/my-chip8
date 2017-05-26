//
//  main.c
//  chip8GL
//
//  Created by Adenike Adeyemi on 5/25/17.
//  Copyright © 2017 Adenike Adeyemi. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include <GLUT/GLUT.h>

#define SCREEN_HEIGHT 32
#define SCREEN_WIDTH 64
#define MODIFIER 10;


static unsigned short drawFlag = 0;

int display_width = SCREEN_WIDTH * MODIFIER;
int display_height = SCREEN_HEIGHT * MODIFIER;

void renderScene();
void changeSize(int x, int y);
void normalKeys();
void setupTexture();

uint8_t screenData[SCREEN_HEIGHT][SCREEN_WIDTH][3]; //screen data [Y][X][color]
Chip8 chip8;

int main(int argc, const char * argv[]) {
    
    if(argc < 2){
        printf("Usage: %s <bin file>\n", argv[0]);
        exit(1);
    }
    
    //initialize chip 8
    chip8_Init();
    //load file if exists
    chip8_load(argv[1]);
    
    //setup graphics
    glutInit(&argc, argv);
    glutInitWindowSize(display_width, display_height);
    glutInitWindowPosition(320, 320);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    glutCreateWindow("Chip8 GL");
    
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    
    setupTexture();
    
    glutMainLoop();
    
    //setup inputs
    
    
    
    //    //show memory map
    //    //chip8_Debug_Display_Memory();
    //    //run file for 60 cycles
    //    for(;;){
    //        //one fetch - execute cycle
    //        chip8_emulateCycle();
    //
    //        //display registers
    //        //chip8_Debug_Display_All_Register();
    //
    //        //display Stack
    //        //chip8_Debug_Display_Stack();
    //
    //        if(chip8_Debug_Get_Opcode() == 0x0000)
    //            break;
    //
    //        //draw to screen
    //
    //        //get key state
    //    }
    //    //chip8_Debug_Display_Memory();
    //
    
    return 0;
}

//Glut and graphics drawing

void setupTexture(){
    //clear screen
    for(int y = 0; y < SCREEN_HEIGHT; y++){
        for(int x = 0; x < SCREEN_WIDTH; x++){
            for(int rgb = 0; rgb < 3; rgb++){
                screenData[y][x][rgb] = 0;
            }
        }
    }
    //create texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 (GLvoid*)screenData);
    
    //setup texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    //enable texture
    glEnable(GL_TEXTURE_2D);
}

void updateTexture(){
    //update pixels
    for(int y = 0; y < SCREEN_HEIGHT; y++){
        for(int x = 0; x < SCREEN_WIDTH; x++){
            //colors pixels based on gfx state
            if(chip8.gfx[(y * SCREEN_WIDTH)+x] == 0){
                for(int rgb = 0; rgb < 3; rgb++){
                    screenData[y][x][rgb] = 0;
                }
            }
            else{
                for(int rgb = 0; rgb < 3; rgb++){
                    screenData[y][x][rgb] = 255;
                }
            }//end of else
        }//end of x loop
    }//end of y loop
    //update textures
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);
    
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0); glVertex2d(0.0          , 0.0);
    glTexCoord2d(1.0, 0.0); glVertex2d(display_width, 0.0);
    glTexCoord2d(1.0, 1.0); glVertex2d(display_width, display_height);
    glTexCoord2d(0.0, 1.0); glVertex2d(0.0          , display_height);
    glEnd();
    
}

//draw screen
void renderScene(){
    chip8_emulateCycle();
    if(drawFlag == 1){
        //clear buffer frame
        glClear(GL_COLOR_BUFFER_BIT);
        
        //draw pixels to texture
        updateTexture();
        
        //swap buffers
        glutSwapBuffers();
        //processed frame
        
        drawFlag = 0;
    }
}

//keep perspective
void changeSize(int x, int y){
    glClearColor(0.0f, 0.0f, 0.5f, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, x, y, 0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, x, y);
    
    display_height = y;
    display_width = x;
    
    
    
}

//key inputs
void normalKeys(){}

//Chip 8 functions
void chip8_Init(){
    
    
    chip8.I = 0;      //clear Index Register
    chip8.sp = 0;     //clear stack pointer
    chip8.opcode = 0; //clear opcodes
    chip8.pc = 0x200; //starting point in mem
    
    //clear memory
    for(int i = 0; i<4096; i++){
        chip8.memory[i] = 0;
    }
    
    //clear registers
    for(int i = 0; i < 16; i++){
        chip8.V[i] = 0;
    }
    
    //clear stack
    for (int i = 0; i < 16; i++) {
        chip8.stack[i] = 0;
    }
    
    //clear display
    for (int i = 0; i<(SCREEN_WIDTH*SCREEN_HEIGHT); i++) {
        chip8.gfx[i] = 0;
    }
    
    
    //set fontset at memory 0x50 or 80
    for(int i = 0; i < 80; i++){
        chip8.memory[i+80] = chip8_fontset[i];
    }
    
    chip8.delay_timer = 0;
    chip8.sound_timer = 0;
    
    
}


//chip 8 functions
void chip8_load(char *filename){
    
    //open file
    FILE *fp;
    unsigned char buffer[3584]; //space for loaded program
    size_t bufferSize;  //size of loaded program
    
    fp = fopen(filename, "rb"); //open file and read it in binary
    if(fp == NULL){
        printf("File %s does not exists", filename); //if file is no valid exit program
        exit(0);
    }
    
    //load data into buffer and get bufferSize
    bufferSize = fread(buffer, 1, 3583, fp);
    
    
    //load program TODO: get buffersize and buffer
    for(int i = 0; i < bufferSize; i++){
        chip8.memory[i+512] = buffer[i];
    }
    
}

void chip8_emulateCycle(){
    
    //draw variables
    unsigned short drawFlag;
    unsigned short x;
    unsigned short y;
    unsigned short height;
    unsigned short pixel;
    
    //Fetch opcode
    chip8.opcode = (chip8.memory[chip8.pc] << 8) | chip8.memory[chip8.pc+1];
    
    //debug print opcode
    printf("Opcode: [0x%x]\n", chip8.opcode);
    printf("pc: 0x%x\n", chip8.pc);
    
    chip8.pc+=2;
    
    //Decode opcode
    switch(chip8.opcode&0xF000){
            
        case 0x0000:
            switch(chip8.opcode&0x0FFF){
                case 0x0E0://00E0 clear display
                    
                    for (int i = 0; i<(SCREEN_WIDTH*SCREEN_HEIGHT); i++) {
                        chip8.gfx[i] = 0;
                    }
                    break;
                case 0x0EE:
                    //TODO: get return pc from stack
                    chip8.sp--;           //move stack pointer down
                    chip8.pc = chip8.stack[chip8.sp]; //get pc from stack
                    break;
                default:// 0x0NNN not needed
                    printf("Opcode [0x000]: Invalid Opcode 0x%x\n", chip8.opcode);
                    break;
            }
            break;
        case 0x1000:
            chip8.pc = chip8.opcode & 0x0FFF;   //1NNN Jump to NNN
            break;
        case 0x2000:
            chip8.stack[chip8.sp]=chip8.pc;           //push program counter into stack
            chip8.sp++;                   //increment stack pointer
            chip8.pc = chip8.opcode & 0x0FFF;   //2NNN jump to function at NNN
            break;
        case 0x3000:
            //3XNN if VX == NN skip
            if(chip8.V[(chip8.opcode&0x0F00)>>8] == (chip8.opcode&0x00FF))
                chip8.pc+=2;
            
            break;
        case 0x4000:
            //4XNN if VX != NN skip
            if(chip8.V[(chip8.opcode&0x0F00)>>8] != (chip8.opcode&0x00FF))
                chip8.pc+=2;
            
            break;
        case 0x5000:
            //5XNN if VX == VY skip
            if(chip8.V[(chip8.opcode&0x0F00)>>8] == chip8.V[(chip8.opcode&0x00F0)>>4])
                chip8.pc+=2;
            
            break;
        case 0x6000:
            //6XNN Vx = NN
            //            printf("In [6000] %d\n", (opcode&0x0F00)>>8);
            chip8.V[(chip8.opcode&0x0F00)>>8] = (chip8.opcode&0x00FF);
            
            break;
        case 0x7000:
            //7XNN Vx += NN
            chip8.V[(chip8.opcode&0x0F00)>>8] += (chip8.opcode&0x00FF);
            break;
        case 0x8000:
            switch(chip8.opcode&0x000F){
                case 0x0000: //8XY0 Vx = Vy
                    chip8.V[(chip8.opcode&0x0F00)>>8] = chip8.V[(chip8.opcode&0x00F0)>>4];
                    break;
                case 0x0001: //8XY1 Vx = Vx | Vy
                    chip8.V[(chip8.opcode&0x0F00)>>8] = chip8.V[(chip8.opcode&0x0F00)>>8] | chip8.V[(chip8.opcode&0x00F0)>>4];
                    break;
                case 0x0002: //8XY2 Vx = Vx & Vy
                    chip8.V[(chip8.opcode&0x0F00)>>8] = chip8.V[(chip8.opcode&0x0F00)>>8] & chip8.V[(chip8.opcode&0x00F0)>>4];
                    break;
                case 0x0003: //8XY3 Vx = Vx ^ Vy
                    chip8.V[(chip8.opcode&0x0F00)>>8] = chip8.V[(chip8.opcode&0x0F00)>>8] ^ chip8.V[(chip8.opcode&0x00F0)>>4];
                    break;
                case 0x0004: //8XY4 Vx += Vy Vx + Vy
                    if(chip8.V[(chip8.opcode & 0x00F0) >> 4] > (0xFF - chip8.V[(chip8.opcode & 0x0F00) >> 8]))
                        chip8.V[15] = 1;
                    else
                        chip8.V[15] = 0;
                    
                    chip8.V[(chip8.opcode&0x0F00)>>8] += chip8.V[(chip8.opcode&0x00F0)>>4];
                    
                    break;
                case 0x0005: //8XY5 Vx -= Vy Vx - Vy
                    if(chip8.V[(chip8.opcode & 0x00F0) >> 4] > (chip8.V[(chip8.opcode & 0x0F00) >> 8]))
                        chip8.V[15] = 0;
                    else
                        chip8.V[15] = 1;
                    chip8.V[(chip8.opcode&0x0F00)>>8] -= chip8.V[(chip8.opcode&0x00F0)>>4];
                    break;
                case 0x0006: //8XY6 Vx>>1
                    chip8.V[15] = chip8.V[(chip8.opcode&0x0F00)>>8] & 0x0001;
                    chip8.V[(chip8.opcode&0x0F00)>>8] = chip8.V[(chip8.opcode&0x0F00)>>8] >> 1;
                    break;
                case 0x0007: //8XY7 Vx = Vy - Vx
                    if(chip8.V[(chip8.opcode & 0x00F0) >> 4] < (chip8.V[(chip8.opcode & 0x0F00) >> 8]))
                        chip8.V[15] = 0;
                    else
                        chip8.V[15] = 1;
                    chip8.V[(chip8.opcode&0x0F00)>>8] = chip8.V[(chip8.opcode&0x00F0)>>4] - chip8.V[(chip8.opcode&0x0F00)>>8];
                    break;
                case 0x000E: //8XYE Vx<<1
                    chip8.V[15] = (chip8.V[(chip8.opcode&0x0F00)>>8] & 0x8000) >> 15;
                    
                    chip8.V[(chip8.opcode&0x0F00)>>8] = chip8.V[(chip8.opcode&0x0F00)>>8] << 1;
                    break;
                default:
                    printf("Opcode [0x8000]: Invalid Opcode 0x%x\n", chip8.opcode);
                    break;
                    
            }
            break;
        case 0x9000: //9XY0 if VX != VY skip
            if(chip8.V[(chip8.opcode&0x0F00)>>8] != chip8.V[(chip8.opcode&0x00F0)>>4])
                chip8.pc+=2;
            break;
            
            
        case 0xA000: //ANNN I = NNN
            chip8.I = chip8.opcode & 0x0FFF;
            break;
        case 0xB000: //BNNN pc = V0 + NNN
            chip8.pc = chip8.V[0] + (chip8.opcode&0x0FFF);
            break;
        case 0xC000: //CXNN VX = rand()&NN
            chip8.V[(chip8.opcode&0x0F00)>>8] = (rand()%256)&(chip8.opcode&0x00FF);
            break;
        case 0xD000: //DXYN draw(Vx, Vy, N)
            x = chip8.V[(chip8.opcode&0x0F00)>>8];
            y = chip8.V[(chip8.opcode&0x00F0)>>4];
            height = chip8.opcode&0x000F;
            chip8.V[15] = 0;
            
            for(int yline = 0; yline < height; yline++){
                pixel = chip8.memory[chip8.I + yline];
                for(int xline = 0; xline < 8; xline++){
                    
                    if((pixel & (0x80>>xline)) == 1){
                        
                        if(chip8.gfx[(x + xline + ((y + yline)*SCREEN_WIDTH))] == 1)
                            chip8.V[15] = 1;
                        chip8.gfx[(x + xline + ((y + yline)*SCREEN_WIDTH))]^=1;
                    }//end draw pixel loop
                }//loop col
            }//loop row
            drawFlag = 1;
            break;
        case 0xE000:
            switch(chip8.opcode&0x00FF){
                case 0x9E: //EX9E if key() == VX
                    if(chip8.key[chip8.V[(chip8.opcode&0x0F00) >> 8]] == 1) //if pressed
                        chip8.pc+=2;
                    break;
                case 0xA1: //EXA1 if key() != VX
                    if(chip8.key[chip8.V[(chip8.opcode&0x0F00) >> 8]] == 0) //if not pressed
                        chip8.pc+=2;
                    break;
                default:
                    printf("Opcode [0xE000]: Invalid Opcode 0x%x\n", chip8.opcode);
                    break;
            }
            break;
        case 0xF000:
            switch(chip8.opcode&0x00FF){
                    
                case 0x07: //FX07 Vx = getDelay Timer
                    chip8.V[(chip8.opcode&0x0F00)>>8] = chip8.delay_timer;
                    break;
                case 0x0A: //FX0A Vx = get_key();
                    //TODO Vx = getkey() do not advance until key press
                    
                    chip8.pc -= 2; //stop advance
                    for(int i = 0; i < 16; i++){        // check all keys
                        if(chip8.key[i] == 1){                //find one that is pressed
                            chip8.V[(chip8.opcode&0x0F00)>>8] = i;  // store key in register
                            chip8.pc += 2;                    //advance
                            break;
                        }//end of if
                    }//end of for
                    break;
                case 0x15: //FX15 delay_timer = Vx
                    chip8.delay_timer = chip8.V[(chip8.opcode&0x0F00)>>8];
                    break;
                case 0x18: //FX18 sound_timer = Vx
                    chip8.sound_timer = chip8.V[(chip8.opcode&0x0F00)>>8];
                    break;
                case 0x1E: //FX1E I += Vx
                    chip8.I += chip8.V[(chip8.opcode&0x0F00)>>8];
                    break;
                case 0x29:
                    //TODO sprite_address(VX)
                    break;
                case 0x33: //FX33 Binary coded decimal
                    chip8.memory[chip8.I] = chip8.V[(chip8.opcode&0x0F00)>>8] / 100;
                    chip8.memory[chip8.I+1] = (chip8.V[(chip8.opcode&0x0F00)>>8] % 100) / 10;
                    chip8.memory[chip8.I+2] = (chip8.V[(chip8.opcode&0x0F00)>>8] % 100) % 10;
                    break;
                case 0x55: //FX55 Stores V0 to VX inclusive in memory I
                    for(int i = 0; i <= ((chip8.opcode&0x0F00)>>8); i++)
                        chip8.memory[chip8.I+i] = chip8.V[i];
                    break;
                case 0x65: //FX65 Loads V0 to VX inclusive from memory I
                    for(int i = 0; i <= ((chip8.opcode&0x0F00)>>8); i++)
                        chip8.V[i] = chip8.memory[chip8.I+i];
                    break;
                default:
                    printf("Opcode [0xF000]: Invalid Opcode 0x%x\n", chip8.opcode);
                    break;
                    
            }
            break;
        default:
            printf("Unknown opcode: 0x%X\n", chip8.opcode);
            break;
            
    }//end of decoding opcodes
    
    
    //update timers
    if(chip8.delay_timer > 0)
    {
        //        printf("%d\n",delay_timer);
        chip8.delay_timer--;
    }
    
    if(chip8.sound_timer > 0){
        if(chip8.sound_timer == 1)
            printf("BEEP!\n");
        
        chip8.sound_timer--;
    }
}

//Debug functions
void chip8_Debug_Display_Memory(){
    
    //0x000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    
    for (int i = 0; i < 4096; i++){
        if(i%16 == 0)
            printf("0x%03x: ", i);
        printf("%02x ", chip8.memory[i]);
        if(i%16 == 15)
            printf("\n");
    }
    
}

void chip8_Debug_Display_All_Register(){
    //printf("pc: 0x%03x\n", pc);
    for(int i = 0; i < 16; i++){
        printf("V%d: Hex: 0x%02x, Dec: %d\n", i, chip8.V[i], chip8.V[i]);
    }
    printf("I: Hex: 0x%0x, Dec: %d\n", chip8.I, chip8.I);
}

void chip8_Debug_Display_Register(int i){
    //printf("pc: 0x%03x\n", pc);
    printf("V%d: Hex: 0x%02x, Dec: %d\n", i, chip8.V[i], chip8.V[i]);
}

unsigned short chip8_Debug_Get_Opcode(){
    return chip8.opcode;
}

void chip8_Debug_Display_Stack(){
    printf("Stack pointer: %d\n", chip8.sp);
    printf("Stack: ");
    for(int i = 0; i < 16; i++){
        printf("0x%04x ", chip8.stack[i]);
    }
    printf("\n");
}

