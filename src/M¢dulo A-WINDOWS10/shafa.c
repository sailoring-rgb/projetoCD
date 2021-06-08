#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "moduloA.h"

int main(int argc, char *argv[]) {

	int aux;

	if (argc >= 4 && !strcmp(argv[2], "-m") && !strcmp(argv[3], "f")){

		aux=lecomandoA(argc, argv);
		if (!aux) printf("Comando não reconhecido.\n");

	}

	else printf("Comando não reconhecido.\n");

	return 0;
}