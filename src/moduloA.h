/**

Autores: João Mateus Costa Coelho (a93255) e Diogo Miguel da Silva Araujo (a93313)

@file moduloA.h
Divisão em blocos, compressão RLE e cálculo de frequências.
*/
#ifndef __MODULO_A_H__
#define __MODULO_A_H__

/**
\brief Tipo de dado criado para guardar uma lista de bytes de um ficheiro e o respetivo tamanho.
@param bloco Array que contém os caracteres guardados num bloco.
@param tamanhoBloco Tamanho correspondente ao array.
*/
typedef struct{

	unsigned char *bloco;
	int tamanhoBloco;
} BLOCO_T;


/**
\brief Tipo de dados criado para guardar uma série de informações úteis ao decorrer do programa, incluindo
uma lista dos tamanhos dos blocos comprimidos, o número de blocos comprimidos, o numero de bolos originais,
um controlador para sabermos se foi ou não comprimido, o tamanho original do ficheiro de Entrada e o tamanho
do ficheiro comprimido.
@param lista_tamanhos_finais Lista com os tamanhos de cada um dos blocos analisados.
@param n_blocos_comprimidos Número de blocos que foram analisados
@param n_blocos Número final de blocos atualizado para o caso de haver blocos que se juntem.
@param controlador 1, caso seja feita a compressão; 0, caso contrário.
@param tamanhoOriginal Tamanho original do ficheiro de ENTRADA.
@PARAM tamanhoComprimido Tamanho Final do ficheiro de entrada após compressão RLE.
*/
typedef struct {
	int *lista_tamanhos_finais;
	int n_blocos_comprimidos;
	int n_blocos;
	int controlador;
	int tamanhoOriginal;
	int tamanhoComprimido;
} CARACT_FICHEIRO_COMP;


/**
\brief Define qual será o tamanho dos blocos a serem analisados.
@param tamanho Iniciais dos tamanhos pré-definidos para posterior análise dos ficheiros de entrada.
@returns int Correspondente ao tamanho.
*/
int tamanhoBloco(char tamanho);


/**
\brief Decide-se se vale a pena fazer uma compressão RLE.
@param original Tamanho original de bytes do ficheiro de ENTRADA.
@param comprimido Tamanho final de bytes após o ficheiro ser comprimido.
@returns int 1, se for para comprimir; 0, caso contrário.
*/
int decisao_Compressao(int original, int comprimido);


/**
\brief Calcula o número de vezes que um caracter aparece repetido, seguidos.
@param array Array de bytes que constam no ficheiro de ENTRADA.
@param tamanhoBloco Tamanho do bloco em questão, a ser analisado.
@param posicao Índice do array no elemento em que se está a verificar se o mesmo se repete.
@returns int Número de repetições seguidas de um caracter.
*/
int teste4(unsigned char *array, int tamanhoBloco, int posicao);


/**
\brief Comprime um bloco para o formato RLE.
@param array Bloco que será comprimido.
@param tamanhoBlocoInicial Tamanho original do bloco a comprimir.
@returns BLOCO_T Bloco comprimido e tamanho do mesmo.
*/
BLOCO_T compress_aux(unsigned char *array, int tamanhoBlocoInicial);


/**
\brief Função principal da primeira parte do modulof, a compressão RLE 
(comprime o ficheiro caso obedeça a certas especificações).
@param nomedoficheiro Nome do ficheiro a ser comprimido.
@param tamanhoBlocoBase Tamanho dos blocos recebido nos argumentos da main.
@param forcarRLE 1, caso seja pedido para forçar a compressão RLE; 0, caso contrário.
@returns CARACT_FICHEIRO_COMP Diversas informações especificadas na criação da struct.
*/
CARACT_FICHEIRO_COMP compress(char *nomedoficheiro, int tamanhoBlocoBase, int forcarRLE);


/**
\brief Mediante houver sido feita uma compressão RLE, ou não, decide como começará o ficheiro RLE {R || N}.
@param fp Ficheiro em que se destina a escrita.
@param compressao 1, caso tenha sido feita comressão RLE; 0, caso contrário.
@param nBlocos Numero de blocos
*/
void Freq_Default(FILE *fp, int compressao, int nBlocos);


/**
\brief Altera o nome do ficheiro de ENTRADA para .freq.
@param nome Nome do ficheiro de ENTRADA.
@returns char * Nome do ficheiro .freq.
*/
char *freq_name(char *nome);


/**
\brief Cria um array de inteiros (255), para fazer a contagem do número de vezes que um byte deverá ser escrito no ficheiro .freq.
@param caracteres Array de caracteres/bytes a ser analisado.
@param bytesPorLer Tamanho do bloco a ser lido em questão.
@returns int * Array de frequências gerado a partir do array de caracteres.
*/
int *Array_freq_por_bloco(unsigned char *caracteres, int bytesPorLer);


/**
\brief Escreve no ficheiro destino as frequências dos caracteres utilizados.
@param fp Ficheiro de destino
@param freqs Array que contém as frequências
@param tamanhoBloco Tamanho do bloco correspondente ao presente array de frequências, também a ser escrito no ficheiro destino.
*/
void escreve_Ficheiro_Freq(FILE *fp, int *freqs, int tamanhoBloco);


/**
\brief Função principal para escrita do ficheiro .freq.
@param dados_Da_Compressao Dados referentes à anterior análise dos blocos do ficheiro de entrada e a respetiva, ou não, compressão.
@param nomedoficheiro Nome do ficheiro de ENTRADA.
@param tamanho_bloco_base Tamanho do bloco especificado na main.
@param listaTBlocos Lista do tamanho dos blocos comprimidos utilizados para o cálculo das frequências.
*/
void frequencias(CARACT_FICHEIRO_COMP dados_Da_Compressao, char *nomedoficheiro, int tamanho_bloco_base, int *listaTBlocos);


/**
\brief Função principal deste módulo A.
@param nomedoficheiro Nome do ficheiro de ENTRADA.
@param tamanho Carater correspondente ao tamanho pretendido para os blocos a serem analisados.
@param forcarRLE 1, caso seja suposto forçar a compressão RLE; 0, caso conrário.
*/
void moduloA(char *nomedoficheiro, char tamanho, int forcarRLE);


/**
\brief Função aplicadora dos argumentos fornecidos pela consola.
@param argc Numero de argumentos.
@param argv Argumentos fornecidos.
@returns int 1 caso os argumentos tenham sido validados; 0 caso contrário.
*/
int lecomandoA(int argc, char *argv[]);


#endif //MODULO_A_H