#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "moduloD.h"



int main(int argc, char *argv[]){
    if(!strcmp(argv[3], "d")||!strcmp(argv[3], "-d")){
        if(lecomandoD(argc,argv)==-1)
        printf("Command is not recognized");
    }
    else printf("Command is not recognized");
    
    return 0;
}
