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
    
    fp = fopen("test6.bin", "wb");
    if(!fp){
        printf("Unable to open\n");
        return 1;
    }
//    char x[10]="ABCDEFGHIJ";
    char x[12] =
    {
        0xA0, 0x50,
        0x60, 0x10,
        0x61, 0x18,
        0xD0, 0x05,
        0xA0, 0x5a,
        0xD1, 0x05
        
    };
    fwrite(x, sizeof(x[0]), sizeof(x)/sizeof(x[0]), fp);
    fclose(fp);

}
