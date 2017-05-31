//
//  binary.c
//  
//
//  Created by Adenike Adeyemi on 5/18/17.
//
//

#include <stdio.h>

int main(){

    FILE *fp;
    
    fp = fopen("test4.bin", "wb");
    if(!fp){
        printf("Unable to open\n");
        return 1;
    }
//    char x[10]="ABCDEFGHIJ";
    //void function1()
    //int function2()
    //int function3(int x, int y)
    char x[88] =
    {
        0xA2, 0x58, //200 set I to end of instructions
        0x60, 0x06, //202 V0 = 3
        0xF0, 0x18, //204 set timer
        0xC1, 0xFF, //206 rand values
        0xC2, 0xFF, //208
        0xC3, 0xFF, //20A
        0xC4, 0xFF, //20C
        0xC5, 0xFF, //20E
        0xC6, 0xFF, //210
        0xC7, 0xFF, //212
        0xC8, 0xFF, //214
        0xC9, 0xFF, //216
        0xCA, 0xFF, //218
        0xCB, 0xFF, //21A
        0xCC, 0xFF, //21C
        0xCD, 0xFF, //21E
        0xCE, 0xFF, //220
        0xFE, 0x55, //222 dump registers in memory
        0xF0, 0x15, //224 set delay
        0xF0, 0x18, //226 set timer
        0x60, 0x03, //228 V0 = 3
        0x61, 0x47, //22A V1 = 71
        0x62, 0xFC, //22C v2 = 252
        0x63, 0x00, //22E 0 everything
        0x64, 0x00, //230
        0x65, 0x00, //232
        0x66, 0x00, //234
        0x67, 0x00, //236
        0x68, 0x00, //238
        0x69, 0x00, //23A
        0x6A, 0x00, //23C
        0x6B, 0x00, //23E
        0x6C, 0x00, //240
        0x6D, 0x00, //242
        0x6E, 0x00, //244
        0xA2, 0x68, //246
        0xF0, 0x33, //248
        0xA2, 0x78, //24A
        0xF1, 0x33, //24C
        0xA2, 0x88, //24E
        0xF2, 0x33, //250
        0xA2, 0x58, //252
        0xFE, 0x65, //254
        0x00, 0x00  //256
        
       
    };
    fwrite(x, sizeof(x[0]), sizeof(x)/sizeof(x[0]), fp);
    fclose(fp);

}