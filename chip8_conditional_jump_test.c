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
    
    fp = fopen("test2.bin", "wb");
    if(!fp){
        printf("Unable to open\n");
        return 1;
    }
//    char x[10]="ABCDEFGHIJ";
    char x[68] =
    {
        0x60, 0x05, //200               V0 = 0x05
        0x61, 0x08, //202               V1 = 0x08
        0x62, 0x04, //204               V2 = 0x04
        0x63, 0x02, //206               V3 = 0x02
        0x64, 0x01, //208               V4 = 0x01
        0x65, 0x00, //20A               V5 = 0x00
        0x66, 0x00, //20C               V6 = 0x00
        0x67, 0x00, //20E               V7 = 0x00
        0x68, 0x00, //210               V8 = 0x00
        0x55, 0x00, //212   loop:       if(V5 == V0)
        0x12, 0x18, //214               goto branch1
        0x12, 0x44, //216               goto end
        0x32, 0x06, //218   branch1:    if V2 == 6
        0x12, 0x20, //21A               goto false1
        0x66, 0x06, //21C   true1:      V6 = 6
        0x12, 0x22, //21E               goto false1 + 2
        0x72, 0x01, //220   false1:     V2 += 1
        0x42, 0x06, //222   branch2:    V2 != 6
        0x12, 0x2A, //224               goto false2
        0x81, 0x45, //226   true2:      V1 -= V4
        0x12, 0x2C, //228               goto false2 + 2
        0x81, 0x44, //22A   false2:     V1 += V4
        0x51, 0x20, //22C   branch3:    if(V1 == V2)
        0x12, 0x34, //22E               goto false3
        0x87, 0x20, //230   true:       V7 = V2
        0x12, 0x36, //232               goto false3 + 2
        0x87, 0x24, //234   false3:     V7 += V2
        0x97, 0x50, //236               if(V7 != V5)
        0x12, 0x3E, //238               goto false4:
        0x88, 0x14, //23A   true4:      V8 += V1
        0x12, 0x40, //23C               goto false4 + 2
        0x88, 0x50, //23E   false4:     V8 = V5
        0x75, 0x01, //240               V5 += 1
        0x12, 0x12, //242               jump to loop
        
        
    };
    fwrite(x, sizeof(x[0]), sizeof(x)/sizeof(x[0]), fp);
    fclose(fp);

}
