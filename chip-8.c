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

void setupTexture();
void updateTexture();
void renderScene();
void changeSize();
void chip8_Debug_Display_Screen();
void chip8_keypad(unsigned char key, int x, int y);
void chip8_keypadUp(unsigned char key, int x, int y);

#define SCREEN_HEIGHT 32
#define SCREEN_WIDTH 64
#define MODIFIER 10;

static GLubyte screenData[SCREEN_HEIGHT][SCREEN_WIDTH][3];

int display_width = SCREEN_WIDTH * MODIFIER;
int display_height = SCREEN_HEIGHT * MODIFIER;

static GLuint texName;

static unsigned short drawFlag = 0;

Chip8 chip8;

void makeCheckImage(void)
{
    int i, j, c;
    
    for (i = 0; i < SCREEN_HEIGHT; i++) {
        for (j = 0; j < SCREEN_WIDTH; j++) {
            c = ((((i&0x8)==0)^(((j&0x8))==0)))*255;
            screenData[i][j][0] = (GLubyte) c;
            screenData[i][j][1] = (GLubyte) c;
            screenData[i][j][2] = (GLubyte) c;
        }
    }
}

int main(int argc,  char * argv[]) {
    
        if(argc < 2){
            printf("Usage: %s <bin file>\n", argv[0]);
            exit(0);
        }
    
    //initialize chip 8
    chip8_Init();
    //load file if exists
    chip_load(argv[1]);
    //chip8_load("VBRIX");
    
    //setup graphics
    glutInit(&argc, argv);
    glutInitWindowSize(display_width, display_height);
    glutInitWindowPosition(320, 320);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    glutCreateWindow("Chip8 GL");
    
    setupTexture();
    
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(chip8_keypad);
    glutKeyboardUpFunc(chip8_keypadUp);
    
    glutMainLoop();
    
    return 0;
}
//Input
void chip8_keypadUp(unsigned char key, int x, int y){
    switch (key) {
        case 0x31:
            chip8.key[1] = 0;
            break;
        case 0x32:
            chip8.key[2] = 0;
            break;
        case 0x33:
            chip8.key[3] = 0;
            break;
        case 0x34:
            chip8.key[12] = 0;
            break;
        case 'q':
        case 'Q':
            chip8.key[4] = 0;
            break;
        case 'w':
        case 'W':
            chip8.key[5] = 0;
            break;
        case 'e':
        case 'E':
            chip8.key[6] = 0;
            break;
        case 'r':
        case 'R':
            chip8.key[13] = 0;
            break;
        case 'a':
        case 'A':
            chip8.key[7] = 0;
            break;
        case 's':
        case 'S':
            chip8.key[8] = 0;
            break;
        case 'd':
        case 'D':
            chip8.key[9] = 0;
            break;
        case 'f':
        case 'F':
            chip8.key[14] = 0;
            break;
        case 'z':
        case 'Z':
            chip8.key[10] = 0;
            break;
        case 'x':
        case 'X':
            chip8.key[0] = 0;
            break;
        case 'c':
        case 'C':
            chip8.key[11] = 0;
            break;
        case 'v':
        case 'V':
            chip8.key[15] = 0;
            break;
        default:
            break;
    }
}
void chip8_keypad(unsigned char key, int x, int y){
    switch(key){
        case 27:
            exit(0);
            break;
        case 0x31:
            chip8.key[1] = 1;
            break;
        case 0x32:
            chip8.key[2] = 1;
            break;
        case 0x33:
            chip8.key[3] = 1;
            break;
        case 0x34:
            chip8.key[12] = 1;
            break;
        case 'q':
        case 'Q':
            chip8.key[4] = 1;
            break;
        case 'w':
        case 'W':
            chip8.key[5] = 1;
            break;
        case 'e':
        case 'E':
            chip8.key[6] = 1;
            break;
        case 'r':
        case 'R':
            chip8.key[13] = 1;
            break;
        case 'a':
        case 'A':
            chip8.key[7] = 1;
            break;
        case 's':
        case 'S':
            chip8.key[8] = 1;
            break;
        case 'd':
        case 'D':
            chip8.key[9] = 1;
            break;
        case 'f':
        case 'F':
            chip8.key[14] = 1;
            break;
        case 'z':
        case 'Z':
            chip8.key[10] = 1;
            break;
        case 'x':
        case 'X':
            chip8.key[0] = 1;
            break;
        case 'c':
        case 'C':
            chip8.key[11] = 1;
            break;
        case 'v':
        case 'V':
            chip8.key[15] = 1;
            break;
    }
}

//Glut and graphics drawing
void setupTexture(){
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    
    //set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    
}

void updateTexture(){
    //update pixels
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            if(chip8.gfx[(i * SCREEN_WIDTH)+j] == 0){
                screenData[i][j][0] = 0;
                screenData[i][j][1] = 0;
                screenData[i][j][2] = 0;
            }
            else{
                screenData[i][j][0] = 255;
                screenData[i][j][1] = 255;
                screenData[i][j][2] = 255;
            }
            
        }
    }
    //update texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH,
                 SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 screenData);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texName);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex2d(0.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex2d(display_width, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex2d(display_width, display_height);
    glTexCoord2f(0.0, 1.0); glVertex2d(0.0, display_height);
    
    glEnd();
    glutSwapBuffers();
    
    glDisable(GL_TEXTURE_2D);
}

void renderScene(){
    chip8_emulateCycle();
    chip8_Debug_Display_All_Register();
    if(drawFlag == 1){
        updateTexture();
        drawFlag = 0;
    }
    
}

void changeSize(int w, int h){
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
    
    // Resize quad
    display_width = w;
    display_height = h;
}

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
        printf("File %s does not exist.\n", filename); //if file is no valid exit program
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
    //unsigned short drawFlag;
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
    
    //loop back to main if end of memory reached
    if(chip8.pc >= 0xFFF)
        chip8.pc = 0x200;
    
    //Decode opcode
    switch(chip8.opcode&0xF000){
            
        case 0x0000:
            switch(chip8.opcode&0x0FFF){
                case 0x0E0://00E0 clear display
                    
                    for (int i = 0; i<(SCREEN_WIDTH*SCREEN_HEIGHT); i++) {
                        chip8.gfx[i] = 0;
                    }
                    drawFlag = 1;
                    break;
                case 0x0EE:
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
            chip8.stack[chip8.sp]=chip8.pc;     //push program counter into stack
            chip8.sp++;                         //increment stack pointer
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
                    chip8.V[(chip8.opcode&0x0F00)>>8] |= chip8.V[(chip8.opcode&0x00F0)>>4];
                    break;
                case 0x0002: //8XY2 Vx = Vx & Vy
                    chip8.V[(chip8.opcode&0x0F00)>>8] &= chip8.V[(chip8.opcode&0x00F0)>>4];
                    break;
                case 0x0003: //8XY3 Vx = Vx ^ Vy
                    chip8.V[(chip8.opcode&0x0F00)>>8] ^= chip8.V[(chip8.opcode&0x00F0)>>4];
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
                    chip8.V[15] = (chip8.V[(chip8.opcode&0x0F00)>>8]) >> 7;
                    
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
            chip8.V[15] = 0; //collision flag
            
            if(x > 64 || y > 32)
            {
                printf("We got a problem\n");
            };
            
            for(int yline = 0; yline < height; yline++){
                pixel = chip8.memory[chip8.I + yline];
                for(int xline = 0; xline < 8; xline++){
                    
                    if((pixel & (0x80>>xline)) != 0){
                        
                        if(chip8.gfx[(x + xline + ((y + yline)*SCREEN_WIDTH))] == 1)
                            chip8.V[15] = 1;
                        chip8.gfx[(x + xline + ((y + yline)*SCREEN_WIDTH))]^=1;
                    }//end draw pixel process
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
                    chip8.pc -= 2; //stop advance
                    for(int i = 0; i < 16; i++){        // check all keys
                        if(chip8.key[i] == 1){                //find one that is pressed
                            printf("Found key %d", i);
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
                    //I = sprite_address(VX)
                    x =(chip8.opcode&0x0F00)>>8;
                    y = 80+chip8.V[x] * 5;
                    chip8.I = y;
                    printf("%x\n", chip8.I);
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

void chip8_Debug_Display_Screen(){
    
    //0x000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    
    for (int i = 0; i < (SCREEN_WIDTH*SCREEN_HEIGHT); i++){
        
        printf("%02x", chip8.gfx[i]);
        if(i%64 == 63)
            printf("\n");
    }
    
}

void chip8_Debug_Display_All_Register(){
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

