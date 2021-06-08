#include "moduloC.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]){


    if(argc == 4 && !strcmp(argv[2],"-m") && !strcmp(argv[3], "c")){

        fPrincipal(argv[1]);
    }

    else printf("Input Error");

    return 0;
}

