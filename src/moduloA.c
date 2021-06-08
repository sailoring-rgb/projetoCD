#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fsize.h"
#include "moduloA.h"



#define KBYTE_D 65536 //64 kbytes
#define KBYTE_K 655360 //640 kbytes
#define MBYTE_m 8388608 //8 mbytes
#define MBYTE_M 67108864 //64 mbytes	
// -> Definição dos tamanhos dos blocos.



//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------



int tamanhoBloco(char tamanho){

	int t;

	switch(tamanho){

		case 'K':
			t = KBYTE_K;
			break;
		case 'm':
			t = MBYTE_m;
			break;
		case 'M':
			t = MBYTE_M;
			break;
		default:
			t = KBYTE_D;
			break;
	}
	return t;

} // -> Devolve o tamanho dos blocos em que se pretende dividir o ficheiro ORIGINAL.



//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------



int decisao_Compressao(int original, int comprimido){

	float compressao = ((float)original - (float)comprimido) / ((float)original) * 100.0;
	if (compressao >= 5.0) return 1;
	else return 0;

} // -> Decide se é para ser feita a compressão RLE ou não ( >= 5%).



//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------



int teste4(unsigned char *array, int tamanhoBloco, int posicao){

	unsigned char base = array[posicao];
	int resultado = 1;

	for (int i=posicao+1; i<tamanhoBloco && resultado < 255 && base==array[i]; i++) resultado++;

	return resultado;

} // -> Numero de vezes que o carater é repetido.



//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------



BLOCO_T compress_aux(unsigned char *array, int tamanhoBlocoInicial){

	int i = 0, aux, iF = 0;
	unsigned char *produtoFinal = malloc((tamanhoBlocoInicial*2+1) * sizeof(unsigned char));									// Array que levará com o bloco comprimido (é necessário alocar tanta memória pois no pior dos caso será lida uma string com 'NULL' seguido de outro carater repetido até ao final)

	while(i < tamanhoBlocoInicial){ 																							// Início da compressão

		aux = teste4(array, tamanhoBlocoInicial, i);

// Caso uma sequencia de carateres iguais seja maior que 4 (comprime)
		if(aux >= 4 || array[i] == 0){
			produtoFinal[iF] = 0;
			produtoFinal[++iF] = array[i];
			produtoFinal[++iF] = aux;
			iF++;
		}

// Caso uma sequência de carateres iguais seja menor que 4 (não comprime)
		else{
			for(int x = 0; x < aux; x++) {
				produtoFinal[iF] = array[i];
				iF++;
			}
		}
		i += aux; 
	}																															// Final da compressão

// Criação do produto retornado
	BLOCO_T *res;
	res = malloc(sizeof(BLOCO_T));
	res->bloco = produtoFinal;
	res->tamanhoBloco = iF; 


	return *res;

} // -> Comprime um bloco.



//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------



CARACT_FICHEIRO_COMP compress(char *nomedoficheiro, int tamanhoBlocoBase, int forcarRLE){

	int controlador_de_tamanho_para_freq = 1;																					// Controlador inicializaod a 1 pois, em principio, o ficheiro será comprimido

// Criação de variáveis úteis
	unsigned char *bloco_Original = malloc(tamanhoBlocoBase * sizeof(unsigned char));											// Bloco transportador de carateres pre-compressão
	unsigned char c;
	int i = 0, 
	    next_step = 1, 																											// Dita a execução da segunda parte da função compress
		tamanhoFinal=0;
	float compressao;																											// Taxa de compressão


// Criação do BLOCO_T
	BLOCO_T *blocoFinal;
	blocoFinal = malloc(sizeof(BLOCO_T));


// Abertura do ficheiro original
	FILE *original = fopen(nomedoficheiro, "rb");


// Criação das variáveis relativas ao tamanho  e execução do fsize
	unsigned long long total;
    long long n_blocks;
    unsigned long size_of_last_block, block_size;

	block_size = tamanhoBlocoBase;
    n_blocks = fsize(original, NULL, &block_size, &size_of_last_block);
    total = (n_blocks-1) * block_size + size_of_last_block;


//CONSOLA
	printf("Número de blocos: %lld\nTamanho dos blocos analisados no ficheiro original: %lld de %lu bytes / 1 de %lu bytes\n",
	n_blocks, n_blocks-1, block_size, size_of_last_block);
//CONSOLA


// Criação do return
	CARACT_FICHEIRO_COMP *carac_comprimido;
	carac_comprimido = malloc(sizeof(CARACT_FICHEIRO_COMP));


// Criação da lista de tamanhos
	int t_blocos_comprimidos[n_blocks];
	int indice_b_compr = 0;


// Compressão
	
	if (total>=1024){																										// Tamanho do ficheiro superior a 1kb

// Criação e abertura do ficheiro .rle
		FILE *final;
		char *name = malloc( (strlen(nomedoficheiro)+4) * sizeof(char));
		strcpy(name, nomedoficheiro);
		strcat(name, ".rle");
		final = fopen(name, "wb");


//CONSOLA
		printf("Compressão RLE: %s\n", name);
//CONSOLA


// 1ª Parte (Teste de compressão)

		if(!forcarRLE) {																									// RLE não é forçado

			if (n_blocks==2 && size_of_last_block < 1024){                    												// Caso só existam 2 blocos e o ultimo é menor do que 1kb assimilamos ambos no mesmo bloco

				unsigned char bloco_Original_aux[total];																	// Bloco com capacidade para todos os carateres do ficheiro original

				for(;i < total; i++) {
					bloco_Original_aux[i] = getc(original);
				} // -> Colocar os carateres no bloco

				*blocoFinal = compress_aux(bloco_Original_aux, total);														// Compressão
				tamanhoFinal += blocoFinal->tamanhoBloco;

				if (decisao_Compressao(i,blocoFinal->tamanhoBloco)==1){														// A compressão é vantajosa

					t_blocos_comprimidos[indice_b_compr] = blocoFinal->tamanhoBloco;        								// Registar o tamanho do bloco comprimido para posterior uso
					indice_b_compr++;

					for(int x = 0; x < blocoFinal->tamanhoBloco; x++){
						fputc(blocoFinal->bloco[x], final);
					} // -> Escrever os carateres no ficheiro final

					compressao = ((float)total - (float)tamanhoFinal) / ((float)total) * 100.0;

//CONSOLA
					printf("Taxa de compressão total: %.3f%c \n", compressao, '%');
					printf("Tamanho dos blocos analisados no ficheiro RLE: %d bytes\n", blocoFinal->tamanhoBloco);
//CONSOLA

					carac_comprimido->n_blocos_comprimidos = 1;

				}
					
				else {																										// A compressão não é vantajosa
					controlador_de_tamanho_para_freq = 0;
					carac_comprimido->n_blocos_comprimidos = 0;

//CONSOLA
					printf("Taxa de compressão insuficiente para a continuação da operação. Operação terminada.\nTamanho dos blocos analisados no ficheiro RLE: %d bytes\n", blocoFinal->tamanhoBloco);
//CONSOLA

				}

				next_step = 0;
				free(blocoFinal->bloco);																					// Passado a 0 pois a segunda parte nao será necessária (caso seja vantajosa a compressão, todo o ficheiro ja foi comprimido)

			} // -> Final do 1º caso



			else{																											// Caso geral (estrutura semalhante ao anterior)

        		for(; i < tamanhoBlocoBase && i < total; i++) {
					bloco_Original[i] = getc(original);
				} // -> Colocar os carateres no bloco

				*blocoFinal = compress_aux(bloco_Original, i);
				tamanhoFinal += blocoFinal->tamanhoBloco;

				if (decisao_Compressao(i,blocoFinal->tamanhoBloco)==1){														// A compressão é vantajosa

					t_blocos_comprimidos[indice_b_compr] = blocoFinal->tamanhoBloco;
					indice_b_compr++;

					for(int x = 0; x < blocoFinal->tamanhoBloco; x++){
						fputc(blocoFinal->bloco[x], final);
					} // -> Escrever os carateres no ficheiro final
					i = 0;

				}

				else {																										// A compressão não é vantajosa
					next_step = 0;
					controlador_de_tamanho_para_freq = 0;
					carac_comprimido->n_blocos_comprimidos = 0;

//CONSOLA
					printf("Taxa de compressão insuficiente para a continuação da operação. Operação terminada.\nTamanho dos blocos analisados no ficheiro RLE: %d bytes\n", blocoFinal->tamanhoBloco);
//CONSOLA	

				}

				free(blocoFinal->bloco);

			}

		}


// 2ª Parte (execução da compressão pelo resto dos blocos)
		if(next_step==1){																									// A compressão é vantajosa ou o RLE foi forçado


// Criação de indice de blocos
			int i_bloco;																									
			if (forcarRLE==1) i_bloco=0;																					// RLE forçado -> começar no bloco 0
			else if (forcarRLE==0 && n_blocks==1) i_bloco=1+n_blocks;														// Caso específico que irá passar todos os passos seguintes a frente
			else i_bloco=1;																									// Casos restantes


// Criação de variável auxiliar
			int menor_1kb;
			if (size_of_last_block<1024) menor_1kb=2;																		// Serão deixados 2 blocos por comprimir para serem comprimido juntos na fase independente
			else menor_1kb=1;																								// Será deixado apena 1 bloco


			for (; i_bloco < n_blocks-menor_1kb; i_bloco++){

				for(; i < tamanhoBlocoBase; i++) {
					bloco_Original[i] = getc(original);
				} // -> Colocar os carateres no bloco

				*blocoFinal = compress_aux(bloco_Original, tamanhoBlocoBase);
				tamanhoFinal += blocoFinal->tamanhoBloco;

				t_blocos_comprimidos[indice_b_compr] = blocoFinal->tamanhoBloco;
				indice_b_compr++;

				for(int y = 0; y < blocoFinal->tamanhoBloco; y++){
					fputc(blocoFinal->bloco[y], final);
				} // -> Escrever os carateres no ficheiro final
				i = 0;
				free(blocoFinal->bloco);
			} //-> Compressão de todos os blocos exceto 1 ou 2 (menor_1kb)


			if (i_bloco==n_blocks-1){																						// Caso menor_1kb==1

				for(; i < size_of_last_block; i++) {
					bloco_Original[i] = getc(original);
				} // -> Colocar os carateres no bloco

				*blocoFinal = compress_aux(bloco_Original, size_of_last_block);
				tamanhoFinal += blocoFinal->tamanhoBloco;

				t_blocos_comprimidos[indice_b_compr] = blocoFinal->tamanhoBloco;
				indice_b_compr++;

				for(int y = 0; y < blocoFinal->tamanhoBloco; y++){
					fputc(blocoFinal->bloco[y], final);
				} // -> Escrever os carateres no ficheiro final

				free(blocoFinal->bloco);

			} // -> Compressão de 1 ultimo bloco


			else if (i_bloco==n_blocks-2){																					//Caso menor_1kb==2

				int tamanhoLastBlock = size_of_last_block+tamanhoBlocoBase;
				unsigned char bloco_Original_aux[tamanhoLastBlock];

				for(; i < tamanhoLastBlock; i++) {
					bloco_Original_aux[i] = getc(original);
				} // -> Colocar os carateres no bloco

				*blocoFinal = compress_aux(bloco_Original_aux, tamanhoLastBlock);
				tamanhoFinal += blocoFinal->tamanhoBloco;

				t_blocos_comprimidos[indice_b_compr] = blocoFinal->tamanhoBloco;
				indice_b_compr++;

				for(int y = 0; y < blocoFinal->tamanhoBloco; y++){
					fputc(blocoFinal->bloco[y], final);
				} // -> Escrever os carateres no ficheiro final

				free(blocoFinal->bloco);

			} // -> Compressão dos 2 ultimos blocos em conjunto


			compressao = ((float)total - (float)tamanhoFinal) / ((float)total) * 100.0;            							// Cálculo da compressão total
				
//CONSOLA
			printf("Taxa de compressão total: %.3f%c \n", compressao, '%');
//CONSOLA


// Criação de variável para uso externo
			int numero_blocos_comprimidos;
			if (size_of_last_block<1024) numero_blocos_comprimidos = n_blocks-1;
			else numero_blocos_comprimidos = n_blocks;

//CONSOLA			
			printf("Tamanho dos blocos analisados no ficheiro RLE:");

			for (int aux=0; aux<numero_blocos_comprimidos; aux++){
				printf(" %d", t_blocos_comprimidos[aux]);
				if (aux<numero_blocos_comprimidos-1) printf(" /");
			}
			
			printf(" bytes\n");
//CONSOLA

			carac_comprimido->n_blocos_comprimidos = numero_blocos_comprimidos;

		}

		fclose(final);

		if (next_step==0 && size_of_last_block > 1024) remove(name);

		free(name);
	}

	else{																													// Tamanho do ficheiro inferior a 1kb 
		
//CONSOLA
		printf("Tamanho do ficheiro pequeno demais para compressão.\n");
//CONSOLA
	
		carac_comprimido->n_blocos_comprimidos = 0;
		controlador_de_tamanho_para_freq = 0;

	}

	fclose(original);
	free(blocoFinal);
	free(bloco_Original);


// Atribuição de valores para serem enviados para a função moduloA
	carac_comprimido->lista_tamanhos_finais = t_blocos_comprimidos;
	carac_comprimido->n_blocos = n_blocks;
	carac_comprimido->controlador = controlador_de_tamanho_para_freq;
	carac_comprimido->tamanhoOriginal = total;
	carac_comprimido->tamanhoComprimido = tamanhoFinal;

	return *carac_comprimido;

} // -> Função principal que comprime (ou não) o ficheiro em .rle



//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------



void Freq_Default(FILE *fp, int compressao, int nBlocos){

	switch(compressao){
		case 1:
			fprintf(fp, "@R@");	//Caso tenha sido feita a comressão
			break;
		default:
			fprintf(fp, "@N@");	//Caso não tenha sido feita a comressão
			break;
	}
	fprintf(fp, "%d", nBlocos);
} // -> Mediante tenha sido feita a compressão RLE, ou não, decide que inicial deverá escrever e também o número total de blocos



//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------



char *freq_name(char *nome){
	
	char *ficheiro = malloc( (strlen(nome)+5) * sizeof(char));
	strcpy(ficheiro, nome);
	strcat(ficheiro, ".freq");

	return ficheiro;

} // -> Utiliza o nome do ficheiro de ENTRADA para criar o nome do de SAIDA



//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------



int *Array_freq_por_bloco(unsigned char *caracteres, int tamanhoDoBloco){
	
	int *freqs = calloc(255, sizeof(int));

	for(int i = 0; i < tamanhoDoBloco; i++){

		++freqs[caracteres[i]];
	}
	return freqs;

} // -> Cria um array de 255 espaços, para os 255 caracteres possiveis. Inicializa-o com 0's. Lê o array já comprimido e preenche o array de inteiros.



//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------



void escreve_Ficheiro_Freq(FILE *fp, int *freqs, int tamanhoBloco) { 


	fprintf(fp, "@%d@", tamanhoBloco);

	int anterior = freqs[0];
	fprintf(fp, "%d;", freqs[0]);
	
	for(int i = 1; i < 255; i++){
		if(freqs[i] == anterior)
			fprintf(fp, ";");
		else{
			fprintf(fp, "%d;", freqs[i]);
			anterior = freqs[i];
		}
	}

} // -> Tendo o array de inteiros das frequencias, apenas escreve os valores.



//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------



void frequencias(CARACT_FICHEIRO_COMP dados_Da_Compressao, char *nomedoficheiro, int tamanho_bloco_base, int *listaTBlocos){

//DADOS A RETIRAR DOS ARGUMENTOS
	
	int auxiliar_Inteiros = 0,																					//Índice auxiliar do array de inteiros das freqs
		tamanhoOriginal = dados_Da_Compressao.tamanhoOriginal, 													//Tamanho total do ficheiro original
		tamanhoComprimido = dados_Da_Compressao.tamanhoComprimido,												//Tamanho dos blocos quando comprimidos
		control = dados_Da_Compressao.controlador,																//Controlador que indica se foi feita a comressão RLE(1)
		nBlocos;																								//Número de blocos que a compressão gerou

	unsigned char *arrayComprimido;																				//Criar o array dos caracteres que será usado para guardar os carateres dos blocos comprimidos

	//int *tamanhoBloco = dados_Da_Compressao.lista_tamanhos_finais; 												//Array com os tamanhos dos blocos a serem escritos


//DADOS A RETIRAR DOS ARGUMENTOS
	

//ESCRITA EFETIVA NO FICHEIRO .FREQ
	char *nome_freq = freq_name(nomedoficheiro);

	FILE *fp2, *fentrada;
	fp2 = fopen(nome_freq, "wb");																				//Abre o ficheiro [nome] + ".freq"
	fentrada = fopen(nomedoficheiro, "rb");
																						

	if (control==1){																							//Se tiver sido feita a compressão RLE,

		nBlocos = dados_Da_Compressao.n_blocos_comprimidos;														//O número de blocos é o número de blocos após a compressão

		Freq_Default(fp2, control, nBlocos); 																	//Escreve o modelo do ficheiro juntamente com o nº total de blocos que serão escritos.

		for(int i = 0; i < nBlocos; i++){				

			arrayComprimido = malloc(listaTBlocos[i] * sizeof(unsigned char));
		
			for(int aux = 0; aux<listaTBlocos[i]; aux++){
				arrayComprimido[aux] = getc(fentrada);
			}
		

			int *freqs = Array_freq_por_bloco(arrayComprimido, listaTBlocos[i]);								//Cria os arrays de int's para cada bloco						
		

			escreve_Ficheiro_Freq(fp2, freqs, listaTBlocos[i]);													//Escreve esse array no ficheiro de destino (.freq)
		
			free(arrayComprimido);
			free(freqs);																						//Liberta espaços de memória que já não serão utilizados
		}
	}

	else {

		nBlocos = dados_Da_Compressao.n_blocos;

		Freq_Default(fp2, control, nBlocos); 

		arrayComprimido = malloc(tamanho_bloco_base*sizeof(unsigned char));

		int tB = tamanho_bloco_base;

		for(int i = 0; i < nBlocos; i++){

			if (i==nBlocos-1 && tamanhoOriginal%tamanho_bloco_base!=0) tB=tamanhoOriginal%tamanho_bloco_base;
		
			for(int aux = 0; aux<tB; aux++){
				arrayComprimido[aux] = getc(fentrada);
			}

			int *freqs = Array_freq_por_bloco(arrayComprimido, tB);														//Cria os arrays de int's para cada bloco						

			escreve_Ficheiro_Freq(fp2, freqs, tB);																		//Escreve esse array no ficheiro de destino (.freq)
		
			free(freqs);																								//Liberta a posição de memória criada, em questão
		}

		free(arrayComprimido);

	}
	
	fprintf(fp2, "@0");																									//Termina sempre em 0, para indicar que não há mais blocos.

	fclose(fp2);
	free(nome_freq);
	fclose(fentrada);

//ESCRITA EFETIVA NO FICHEIRO .FREQS

} // -> Escreve os ficheiros .freq.



//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------



void moduloA(char *nomedoficheiro, char tamanho, int forcarRLE){

	printf("\n\nJoão Mateus Costa Coelho, a93255, MIEI/CD, 25/12/2020\n");
	printf("Diogo Miguel da Silva Araujo, a93313, MIEI/CD, 25/12/2020\n");
	printf("Modulo: f (calculo das frequencias dos simbolos)\n");


	CARACT_FICHEIRO_COMP controlador_freq;

	clock_t Ticks[2];
	Ticks[0] = clock();

	int t = tamanhoBloco(tamanho);

	controlador_freq = compress(nomedoficheiro, t, forcarRLE);

	int listaTBlocos[controlador_freq.n_blocos_comprimidos];
	for(int d = 0; d < controlador_freq.n_blocos_comprimidos; d++){
		listaTBlocos[d] = controlador_freq.lista_tamanhos_finais[d];
	}


	int controlador_para_print = controlador_freq.controlador;


	if (controlador_freq.controlador==1) { 

		int tamanhoAux = strlen(nomedoficheiro) + 4;
		char name[tamanhoAux];
		strcpy(name, nomedoficheiro);
		strcat(name, ".rle");


		frequencias(controlador_freq, name, t, listaTBlocos);
		

		controlador_freq.controlador = 0;
		free(name);

	}

	frequencias(controlador_freq, nomedoficheiro, t, listaTBlocos);

	Ticks[1] = clock();
	double Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
	printf("Tempo de execução: %g ms\n", Tempo);

	if (controlador_para_print==1) printf("Ficheiros gerados: %s.freq, %s.rle.freq\n", nomedoficheiro, nomedoficheiro);
	else printf("Ficheiros gerados: %s.freq\n", nomedoficheiro);

} // -> Função principal deste módulo A.



//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------



int lecomandoA(int argc, char *argv[]){
	int r=1;

	if (argc==4){
		moduloA(argv[1], 'D', 0);
	} // -> Variação default do programa.

	else if (argc==6){

		if (!strcmp(argv[4],"-b")){

			if (!strcmp(argv[5],"K")) moduloA(argv[1], 'K', 0);

			else if (!strcmp(argv[5],"m")) moduloA(argv[1], 'm', 0);

			else if (!strcmp(argv[5],"M")) moduloA(argv[1], 'M', 0);

			else r=0;

		} // -> Variações de tamanho do programa.

		else if (!strcmp(argv[4],"-c") && !strcmp(argv[5],"r")) moduloA(argv[1], 'D', 1); // -> Variação paraforçar a compressão.

		else r=0;

	}

	else if (argc==8){

		if (!strcmp(argv[6],"-c") && !strcmp(argv[7],"r")){

			if (!strcmp(argv[4],"-b")){

				if (!strcmp(argv[5],"K")) moduloA(argv[1], 'K', 1);

				else if (!strcmp(argv[5],"m")) moduloA(argv[1], 'm', 1);

				else if (!strcmp(argv[5],"M")) moduloA(argv[1], 'M', 1);

				else r=0;

			} // -> Variações de tamanho do programa sendo forçada a compressão.

			else r=0;			

		}

		else r=0;

	}

	else r=0;

	return r;
}



//-------------------------------------------------------------------------------------------------------------------------------