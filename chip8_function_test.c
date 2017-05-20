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
    //void function1()
    //int function2()
    //int function3(int x, int y)
    char x[58] =
    {
        0xA2, 0x3A, //200 set I to left over space
        0xF0, 0x55, //202 save V0
        0x22, 0x24, //204 countTofive()
        0xF0, 0x65, //206 load V0
        0x60, 0x07, //208 V0 = 7
        0x61, 0x25, //20A V1 = 37
        0x22, 0x2E, //20C add(V0, V1) = V14 = V0 + V1 = 44 = 0x2C
        0x8D, 0xE0, //20E V13 = V14
        0x60, 0x43, //210 V0 = 67
        0x61, 0x64, //212 V1 = 100
        0x62, 0x1E, //214 V2 = 30
        0x22, 0x34, //216 add2(V0, V1, V2)
        0x8C, 0xE0, //218 V12 = V14
        0xA2, 0x3A, //21A set I to left over space
        0xF0, 0x55, //21C save V0
        0x22, 0x24, //21E countTofive()
        0xF0, 0x65, //220 load V0
        0x00, 0x00, //222 end
        
        0x60, 0x00, //224 countTofive: V0 = 0
        0x40, 0x05, //226 loop: if(V0 != 5)
        0x00, 0xEE, //228 return
        0x70, 0x01, //22A V0 += 1
        0x12, 0x26, //22C goto loop
        
        0x8E, 0x00, //22E add: V14 = V0
        0x8E, 0x14, //230 V14+= V1
        0x00, 0xEE, //232 return
        
        0x22, 0x2E, //234 add2: add(V0, V1)
        0x8E, 0x24, //236 V14+=V2
        0x00, 0xEE, //238 return
        
    };
    fwrite(x, sizeof(x[0]), sizeof(x)/sizeof(x[0]), fp);
    fclose(fp);

}
