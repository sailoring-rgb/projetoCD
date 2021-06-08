/**
@file moduloD.h
Definição do estado e das funções que o manipulam
*/

#ifndef __MODULO_D_H__
#define __MODULO_D_H__
#include <stdio.h>

/**
\brief Altera o nome dos ficheiros a partir da primeira ocurrencia do changethis por forthis.
É inicialmente utilizado um strcpy para a string base nao ser alterada.
Depois com strstr é guardado em i o apontador da primeira aparição do changethis
Finalmente é utilizado um ultimo strcpy com o apontador i para mudar o changethis por forthis.
*/
char* filenameChange (char *base,char *changethis,char *forthis,char *returnme);
/**
\brief Tipo de variavel usada para optimizacao da descodificacao.
O caminho entre o apontador inicial até ao final vai representar um numero binario.
A variavel code indica o elemento guardado(-1 se estiver vazia).
*/
typedef struct nodoarv {
    int code;
    struct nodoarv *zero, *um;
} *ABin;

/**
\brief Incializa um novo ramo para a arvore binaria.
Os apontadores começam a NULL a variavel code a -1.
*/
ABin newNodo ();

/**
\brief Adiciona um novo elemento a arvore.
A medida que vai procurando a nova posicao a variavel i vai aumentando.
Vai criar novos ramos a medida que avança nos ramos ja existentes.
A funcao acaba quando i=tam e ai escreve a variavel codigo em code (no ramo em que acabou).
*/
void addOrd (ABin *a, char *x,int i,int tam,int codigo);

/**
\brief Procura um elemento na arvore.
A medida que vai procurando a nova posicao a variavel i vai aumentando.
Quando o i for igual a tam a funcao da return a variavel code.
*/
int procura (ABin *a, char *x,int i,int tam);

/**
\brief Coloca todos os elementos do array uma posição para a frente(ate ao indice I).
O ultimo elemento vai para o inicio.
*/
void rodaIndice(char *arr,int indice);

/**
\brief Verifica o conteudo entre dois "@".
O counter é onde vai ser guardado o conteudo.
E usado o atoi para veficar se o conteudo sao inteiros ou um char.
O i acaba na posição do segundo "@".
Por isto é feito i++ no de algumas da utilizacoes desta funcao.
*/
void contachars(unsigned char *buff,int *counter,int *i);

/**
\brief Verifica o conteudo entre dois "@" num file.
O counter é onde vai ser guardado o conteudo.
O ficheiro le ate ao ultimo "@".
Por isto é feito fgetc(file) no fim de algumas da utilizacoes desta funcao.
*/
char contaFILEchars(FILE *file,int *counter,char c);
/**
\brief Recebe um inteiro e retorna um binario.
O binario começa igual a 0.
Faz-se o resto da divisao por 2 do decimal e dividi-se o decimal por 2.
E feita a soma do binario com o resto e multiplica-se por i que aumenta (*10) em cada iteração,
(isto é feito para representar cada casa decimal).
Repete-se até o decimal for igual 0.
*/
int convertdecbin(int dec);
/**
\brief Le o ficheio text de tamanho size e coloca no buff.
Esta funcao abre o ficheiro text com a opcao rb.
Le ate size chars do ficheiro e guarda em bytes os chars lidos.
No fim fecha o ficheiro.
*/
int readFile(int *bytes,char *text,unsigned char **buff,int size);
/**
\brief Leitura de um ficheiro freq.
Vai fazer 2 contachars até chegar ao '@' em que começa a dizer o tamanho do primeiro bloco.
Depois é utilizado um for com duração de nblocos (calculados no segundo contachars),
que faz um primeiro contachars 
*/
int calculaRLETam(char *freq,int **tamblocoRLE,int *nblocosFreq);
/**
\brief Ordena no array a sequencia da codificação RLE.
Simb vai ser o char a ser imprimido.
Count é o numero de vezes seguidas que simb vai aparacer no arrray.

*/
void seqRLE(unsigned char *buff, int posbuff, unsigned char *output, int *posout);
/**
\brief Descodificação de um bloco RLE.
Vai ler todos os chars do bloco.
Caso um dos chars lidos seja =0 então vai iniciar o seqRLE para descomprimir a sequencia RLE.
*/
int decodeBlocoRLE (unsigned char *output,unsigned char *buff, int tamanhoB);
/**
\brief Le o ficheiro COD e guarda a informacao numa arvore.
Vai ler todos os chars entre os ';' nbloco vezes e vai ordenalos na arvore.
Descrito profundamente no codigo.
*/
int decodeCOD (char *cod,int *nblocos,int **tamanhoF,ABin **tree);
/**
\brief Faz a descodificacao SF para um bloco.
A variavel stack guarda a informacao lida de cada posicao do array buff3(onde tem os chars em binario).
A variavel value retira chars da stack.
Quando o tamstack for <8 entao é preciso adicionar 0 no inicio da stack para que o tamanho seja 8.
Para isto é usada a roda indice.
Se a variavel belongs for diferente de "-1" entao o value é um codigo binario pertencente a tree.
Logo esta é inicializada a -1.
Quando o value for um codigo binario da tree entao o belongs vai devolver o valor de tree->code do binario correspondente.
Quando codigo for encontrada, vai ser guardado no array final.
*/
void decodeBLOCOSHAF (int tamBloco,ABin tree,unsigned char *final,int *buff3);
/**
\brief Faz a descodificacao RLE de todo o ficheiro e depois imprime.
Faz leitura e descompressao bloco a bloco.
Descrito profundamente no codigo.
*/
int decoderle (char *rle,char *freq,char *unzipedName,int nblocos,int **tamanhoI,int **tamanhoRLE, int info);
/**
\brief Faz a descodificacao SF de todo o ficheiro e caso info==0 tambem a RLE e depois imprime.
Descrito profundamente no codigo.
*/
int decodeSHAF (char *shaf,char *unzipedName,int *nblocos,int **tamanhoI,int *tamanhoF,int *tamanhoRLE,ABin *tree,int info);
/**
\brief Analisa e informacao do ficheiro COD e depois descodifica o ficheiro shaf.
*/
int decodesf (char *cod,char *shaf,char *unzipedName,int *nblocos,int **tamanhoI,int **tamanhoF,int **tamanhoRLE,int info);
/**
\brief Funcao principal do moduloD.c.
Inicializa os arrays onde vai ser guardada a informacao do tamanho inicial e final dos blocos.
Faz a contagem de descodificacao.
Imprime informacao no terminal.
*/
int decode (char *cod,char *shaf,char *rle, char*freq,char *unzipedName,int tam);
/**
\brief Leitura dos comandos escritos no terminal
A variavel info pode tomar 4 valores.
Quando info==0 o programa recebe um ficheiro .rle.shaf e realiza ambas as descompressoes.
Quando info==-1 o programa recebe um ficheiro .shaf e realiza a descompressao SF
Quando info==-2 o programa pode receber um ficheiro .rle.shaf ou apenas .shaf e vai realizar a opereção SF.
Quando info==-3 o programa recebe ficheiros .rle e realiza descompressao RLE.
No caso de info==-3 a variavel cod é na verdade um nome de um ficheiro .rle.freq.
*/
int lecomandoD(int argc, char *argv[]);
#endif //MODULO_D_H
