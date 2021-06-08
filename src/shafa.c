#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "moduloA.h"
#include "moduloB.h"
#include "moduloC.h"
#include "moduloD.h"

int main(int argc, char *argv[]) {

	int aux;

	if (argc >= 4 && !strcmp(argv[2], "-m") && !strcmp(argv[3], "f")){

		aux=lecomandoA(argc, argv);
		if (!aux) printf("Comando não reconhecido.\n");

	} // Modulo A

	else if(argc==4 && !strcmp(argv[3], "t")){
        printf("Encode \n");
        encode(argv[1]);
        printf("Encode end");
    } // Modulo B

    else if(argc == 4 && !strcmp(argv[2],"-m") && !strcmp(argv[3], "c")){

        fPrincipal(argv[1]);

    } // Modulo C

    else if(!strcmp(argv[3], "d")||!strcmp(argv[3], "-d")){
        if(lecomandoD(argc,argv)==-1)
        printf("Comando não reconhecido.\n");
    } // Modulo D

	else printf("Comando não reconhecido.\n");

	return 0;
}