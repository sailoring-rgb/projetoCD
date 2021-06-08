#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "moduloB.h"
#include "moduloB.c"

int main(int argc, char *argv[]){
    if(argc==4 && !strcmp(argv[3], "t")){
        printf("Encode \n");
        encode(argv[1]);
        printf("Encode end");
    }
    else printf("Command is not recognized");
    
    return 0;
}
    
  