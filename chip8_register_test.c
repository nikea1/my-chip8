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
    
    fp = fopen("test.bin", "wb");
    if(!fp){
        printf("Unable to open\n");
        return 1;
    }
//    char x[10]="ABCDEFGHIJ";
    char x[60] =
    {
        0x60, 0x0F, //600F  V0   = 0F
        0x61, 0xF0, //61F0  V1   = F0
        0x62, 0x3C, //623C  V2   = 3C
        0x63, 0xC0, //63C3  V3   = C0
        0x73, 0x03, //7303  V3  += 03 = C3
        0x84, 0x00, //8400  V4   = V0 = 0F
        0x85, 0x10, //8510  V5   = V1 = F0
        0x86, 0x20, //8620  V6   = V2 = 3C
        0x87, 0x30, //8730  V7   = V3 = C3
        0x84, 0x21, //8421  V4  |= V2 = 0F|3C = 3F
        0x85, 0x42, //8542  V5  &= V4 = F0&3F = 30
        0x86, 0x73, //8673  V6  ^= V7 = 3C^C3 = FF
        0x67, 0x01, //6701  V7   = 01
        0x88, 0x60, //8860  V8   = V6 = FF
        0x89, 0x00, //8900  V9   = V0 = 0F
        0x89, 0x74, //8974  V9  += V7 = 0F + 01 = 10 VF = 0
        0x88, 0x74, //8874  V8  += V7 = FF + 01 = 00 VF = 1
        0x88, 0x75, //8875  V8  -= V7 = 00 - 01 = FF VF = 0
        0x88, 0x75, //8875  V8  -= V7 = FF - 01 = FE VF = 1
        0x89, 0x75, //8975  V9  -= V7 = 10 - 01 = 0F VF = 1
        0x89, 0x06, //8906  V9  >> 1  = 07 VF = 1
        0x6A, 0x7F, //6A7F  V10  = 7F
        0x88, 0xA2, //88A2  V8  &= V10 = FE&7F = 7E
        0x88, 0x0E, //880E  V8  << 1   = FC
        0x88, 0x97, //8897  V8   = V9 - V8 = 07 - FC VF = 0
        0xCB, 0x01, //CB01  V11  = rand(1)
        0xCB, 0x01, //CB01  V11  = rand(1)
        0xCB, 0x20, //CB20  V11  = rand(20)
        0xCB, 0x0F, //CB0F  V11  = rand(0F)
        0xCB, 0xFF  //CBFF  V11  = rand(FF)
        
    };
    fwrite(x, sizeof(x[0]), sizeof(x)/sizeof(x[0]), fp);
    fclose(fp);

}
