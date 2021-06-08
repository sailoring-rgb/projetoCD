/**
Autores: Diogo da Costa e Silva Lima Rebelo (a93180) e Joel Costa Araújo (a76603)
    As nossas funçoes principais foram a fPrincipal e a Conversão
    A fPrincipal,que recebe o ficheiro .txt ou . rle e devolve um ficheiro .shaf, pois e nessa que lemos os ficheiros, inicializamos
    os arrays que irao conter esses mesmos ficheiros, iniciamos o temporizador e iniciamos todas as outras funções auxiliares.
    E aí que tambem escrevemos as informações finais, como por exemplo a taxa de compressão global.
    Na conversão começamos por inicializar e alocar espaco para a matriz que conterá a sequencia binaria do bloco que se esta a trabalhar.
    O nosso objetivo seria conseguirmos alcançar uma string com a sequencia binaria separada bit a bit (ex [1,0,0] em vez de [10,0]).
    Como estavamos a substituir um caracter especifico no bloco inteiro (substituir de uma vez só todos os 'a'), a nossa decisão foi
    criar uma matriz com a quantidade de linhas igual ao numero de caracteres do bloco e assim substituir nas linhas especificas atraves
    da função auxiliar substitui_matriz (ex aabd [[1,0],[1,0],[0],[1,1,0]]). Deste modo foi possível, após o completamento da matriz,
    converter isso numa string com a sequencia binaria separada bit a bit, como pretendiamos, facilitando mais tarde a codificação.
*/

/**
@file moduloC.h
Definição do estado e das funções que o manipulam.
*/

#ifndef __MODULO_C_H__
#define __MODULO_C_H__
#include <stdio.h>
#define BUFFSIZ
/**
\brief Funçao para limpar um array caso seja necessario re-utilizar.
Necessario para a função tamanho_blocos, ficheirotxt.\
@param  *reset array que se pretende limpar.
@param  tamanho tamanho do array.
*/
void reset_array(char *reset, int tamanho);


/**
\brief Recebendo um array contendo o tamanho dos blocos e o número de blocos,
verifica qual é o bloco com o maior tamanho.\
@param tamanhos array com os tamanhos de blocos.
@param n_blocos número total de blocos.
@return maior   inteiro do maior bloco do array.
*/
int maior_numero(int *tamanhos, int n_blocos);


/**
\brief Recebe um array e um inteiro para transformar esse array num inteiro.
Devolve um inteiro.
Ex: [1; 1; 0;] -> int 110;
Função utilizada para agrupar os binarios.\
@param *numeros_string array de números que se pretende que seja um inteiro.
@param contador contador para saber tamanho e em que posiçao do array se encontra.
@return resultado valor do array em inteiro.
*/
int calcula_array(char *numeros_string, int contador);


/**
\brief Percorre um bloco para calcular o tamanho.\
@param *arrayCod array de caracteres do ficheiro ".cod".
@param n_blocos número de blocos.
@param *tamanhos array onde vai ser guardado os tamanhos de cada bloco.
*/
void tamanho_blocos(char *arrayCod, int n_blocos, int *tamanhos);


/**
\brief Guarda num array o conteudo de um bloco.\
@param *arrayCod array de caracteres do ficheiro ".cod".
@param index_cod index do arrayCod onde se pretende começar.
@param *bloco_Cod array para armazenar o conteudo do bloco.
@return devolve o index do final do bloco antes de começar o proximo.
*/
int array_bloco (char *arrayCod, int index_Cod, char *bloco_Cod);


/**
\brief Funçao vai receber o array do bloco original \
@param *bloco_Ori array para armazenar o conteudo do bloco.
@param t_bloco_atual tamanho do bloco a ser percorrido atualmente.
@param posicao em que se encontra o valor_bin no *bloco_Cod. (ex: 'a' está na posiçao 97 porque o ASCII de 'a' é 97).
@param *valor_bin array com o binario correspondente ao caracter em questão.
@param **matriz vai ter colunas com o valor_bin correspondente ao caracter no ficheiro a receber.
(ex: aabb -> [[valor_bin A];[valor_bin A];...[valor_bin B]])
*/
void substitui_matriz(char *bloco_Ori, int t_bloco_atual, char posicao, char *valor_bin, char **matriz);


/**
\brief Função vai agrupar o array dos binarios *str em grupos
de 8 para depois serem convertidos para decimal. Depois de converter imprime o caracter
corresnpondente ao decimal no ficheiro.\
@param *str um array de binarios.
@param str_cnt tamanho do *str
@param *fpShaf File pointer.
oito_char array para agrupar os binarios de 8 em 8.
contador para percorrer o *str.
@param decimal guarda o valor que foi convertido para ser imprimido no ficheiro
*/
void imprime_no_ficheiro (unsigned char *str , int str_cnt, FILE *fpShaf);


/**
\brief Função para criar uma string a partir de uma matriz.
String vai ser uma sequencia de binarios.\
@param **matriz a ser convertida para string.
@param t_bloco_atual tamanho do bloco que esta a ser utilizado.
@param fpShaf File pointer.
*str string onde se pretende armazenar a sequencia binaria.
memory_size contador para o tamanho de memoria que vamos precisar de alocar.
str_cnt contador para o tamanho da string.
@return str_cnt tamanho da string.
*/
int matriz_to_string( char **matriz, int t_bloco_atual, FILE *fpShaf);


/**
\brief Função para calcular quantos zeros serão preciso adicionar ao *str de modo ao str_cnt ser divisivel por 8.\
@param *str string da sequencia binaria.
@param str_cnt tamanho da sequencia.
@return temp numero de 0 que é preciso adicionar.
*/
int quantos_zeros( unsigned char *str, int str_cnt);


/**
\brief A função vai percorrer o bloco_Cod ate encontrar um caracter diferente de ';', guardar o numero binario no array temporaio
 e a sua posicao, que indica qual caracter sera substituido por esse numero binario. Apos isso envia a informação para a função
 substitui_matriz de modo a obter uma matriz com os numeros binarios pela ordem do ficheiro.
@param *bloco_Cod array onde esta guardado o bloco pertencente ao ficheiro Cod que estamos a trabalhar
@param *bloco_Ori array onde esta guardaro o bloco pertencente ao ficheiro Ori que estamos a trabalhar
@param t_bloco_atual tamanho do bloco que estamos a trabalhar
@param *fpShaf Apontador para o ficheiro final
@param maior_bloco tamanho do maior bloco
index_temp contador que percorre o array temporario
index_Cod contador que percorre o bloco_Cod
posicao int que guarda a posicao de um numero binario no bloco_Cod de modo a saber quais sao os caracteres a substituir por esse numero
tamanho_string tamanho da string com a sequencia binaria
temporario array que vai conter temporariamente o numero binario com que trabalhamos
matriz que vai conter os numeros binarios na posiçao correspondente ao ficheiro original (ex: aab [[10],[10],[0]])
@return tamanho_string tamanho da
*/
int conversao(char *bloco_Cod, char *bloco_Ori, int t_bloco_atual, FILE *fpShaf, int maior_bloco);


/**
\brief Função vai escrever os blocos dos ficheiros nos arrays (*bloco_Cod e *bloco_Ori).\
@param *arrayOri array de caracteres do ficheiro original.
@param *arrayCod array de caracteres do ficheiro cod.
@param n_blocos número de blocos total.
@param *tamanhos array com os tamanho dos blocos.
@param maior_bloco tamanho do maior bloco.
@param fpShaf File pointer.
@param *tamanho_blocos_comprimidos array com o tamanho dos blocos comprimidos.
contador_at contador de '@'.
cont_posicao contador para ignorar os primeiros 4 caracteres do ficheiro.
index_array_ori index do array_ori.
bloco_Cod array onde se vai guardar os blocos pertencentes ao ficheiro Cod
bloco_Ori array onde se vai guardar os blocos pertencentes ao ficheiro Ori
*/
void escreve_bloco (char *arrayOri, char *arrayCod, int n_blocos, int *tamanhos, int maior_bloco, FILE *fpShaf, int *tamanho_blocos_comprimidos);


/**
\brief Função para devolver o número de blocos que existem.\
@param *array array de caracteres que foram copiados do ficheiro.
nblocos contador de número de blocos, valor a ser devolvido no final da função.
*temporario array temporario para guardar os caracteres referente a quantidade de blocos que existem.
posicao indica a posicao onde vai começar a ler o array.
index posição no array temporario onde vai ser guardado os caracteres
@return nblocos devolve número de blocos existentes.
*/
int blocos(char *array);


/**
\brief Função para copiar uma string para outra.\
@param *NomeFicheiro1 array que contem nome do ficheiro original.
@param *NomeFicheiro2 array para onde se entende copiar o conteudo do primeiro.
i contador de posição.
*/
void copy_string(char *NomeFicheiro1, char *NomeFicheiro2);


/**
\brief Função para escrever o array com os caracteres do ficheiro.\
@param *fp File pointer.
@param *array array onde se vai guardar os caracteres.
@param size tamanho do ficheiro.
c caracter a ser guardado no array.
*/
void escreve_array(FILE *fp, char *array, int size);


/**
\brief Imprime taxa de compressao e tamanho original do ficheiro para cada bloco.\
@param n_blocos número blocos.
@param *tamanhos lista dos tamanho de cada bloco.
@param *tamanho_comprimido lista dos tamanhos comprimidos
*/
void print_blocos(int n_blocos, int *tamanhos, int *tamanho_comprimido);


/**
\brief Calcula tamanho do ficheiro\
@param *ficheiro ficheiro que se pretende saber o tamanho.
@return total tamanho final do ficheiro.
*/
int fsizee(char *ficheiro);


/**
\brief Função para calcular a taxa total de compressao do ficheiro.\
@param n_blocos número total de blocos.
@param *tamanho_blocos_comprimidos array com tamanho dos blocos depois de sofrerem compresso.
@param sizeOri tamanho do ficheiro original.
tamanho_final vai guarda o valor final da taxa de compressao.
somatorio_tamanhos_comprimido soma o tamanho comprimido de todos os blocos.
@return tamanho_final valor final da taxa de compressao em %.
*/
float taxa_compressao (int n_blocos, int *tamanho_blocos_comprimidos, int sizeOri);


/**
\brief Funçao principal onde se inicializa o temporizador, lê e criam ficheiros e se imprimem as informações finais.\
@param *nomeFicheiroOriginal array que guarda o nome do ficheiro Original
clock_t guarda o número de ticks que o temporizador vai ter
n_blocos número de blocos que um ficheiro tem.
sizeOri tamanho do ficheiro original.
sizeCod tamanho do ficheiro ".cod".
maior_bloco tamanho do maior bloco.
*nomeFicheiroCod guarda o nome do ficheiro original com ".cod".
*nomeFicheiroGerado guarda o nome do ficheiro que vai ser gerado ".shaf".
*arrayOri array para guardar o conteudo do ficheiro original.
*arrayCod array para guardar o conteudo do ficheiro Cod.
*fpointerOri File pointer para o ficheiro original.
*fpointerCod File pointer para o ficheiro ".cod".
*fpShaf File pointer para o ficheiro final ".shaf".
*tamanhos array com os tamanhos dos blocos.
*tamanhos_blocos_comprimidos array com os tamanhos comprimidos dos blocos.
tamanho_final valor final da taxa de compressao em %.
Tempo tempo de execução em milissegundos.
*/
void fPrincipal(char *nomeFicheiroOriginal);
#endif //MODULO_C_H
