/**
Autores: Ana Filipa Ribeiro Murta (a93284) e Ana Paula Oliveira Henriques(a93268)

Neste módulo as funções principais são a calcular_codigos_SF e a encode.

A calculate_codes_SF, tal como o nome indica, calcula os códigos SF recebendo uma lista de frequências de tipo inteiro.
Depois de calculados os códigos para cada frequência, estes são guardados no array codes do tipo unsigned char.
Para além disto, este algoritmo utiliza a variável div para guardar o valor da melhor divisão da lista de frequências.
Já as variáveis start e end limitam a lista de frequências para qual se vai calcular os códigos SF.
O valor destas variáveis vai se alterando de modo a determinar os códigos para cada frequência.
Isto é, de acordo com o div, calculado pela função calculate_best_divider,
no array codes vamos colocar o bit ‘0’ nas posições entre os índices start e div
e o bit ‘1’ entre os índices div+1 e end, aplicando a função calculate_codes_SF recursivamente para estes subconjuntos.

A encode recebe o ficheiro .freq e retorna o ficheiro .cod. 
Inicialmente, esta função guarda o conteúdo do ficheiro num buffer em memória. 
Em seguida, será processado um bloco de cada vez. 
A estratégia utilizada neste algoritmo é ir buscar o conteúdo entre dois arrobas utilizando a função count_char_arroba. 
Deste modo, para além desta função, a encode também utiliza esta estratégia para ir buscar as frequências e colocá-las num array, 
designado por listFreq, que depois irá ser passado como argumento para a função sort_list e sucessivamente para a calculate_codes_SF. 
Por último, o array codes calculado pela calculate_codes_SF é utilizado para construir o ficheiro .cod. 
Assim sendo,  encode é a função que invoca, indiretamente, todas as funções deste módulo, isto é, por exemplo a função swap é utilizada 
pela sort_list, que por sua vez é chamada pela encode.

*/


/**
@file moduloB.h
Definição do estado e das funções que o manipulam
*/

#ifndef __MODULO_B_H__
#define __MODULO_B_H__
#include <stdio.h>

#define MAXCHAR 140000
#define PALAVRAS 257

/**
\brief Função que troca a ordem de dois valores dentro do array
aux é a variável que guarda temporariamente um valor do array
*/
void swap(int array[], int x, int y);


/**
\brief Função devolve como resultado a soma das frequências do array, dando a posicao inicial e a posiçao final
listFreq é o array das frequências
index para percorrer o array das frequências
min é index no array das frequências onde se começa a soma
max é o index no array das frequências onde se para a soma
*/  
int add(int listFreq[],int min, int max);


/**
\brief Função que guarda num array os códigos Shannon-Fano, colocando o bit dado no array
codes é o array onde são guardados os códigos Shannon-Fano
start é o index utilizado para percorrer o array
end é o index utilizado para parar o start de percorrer o array
*/
void add_bit_to_code(unsigned char *bit, unsigned char *codes[], int start, int end);


/**
\brief Função que ordena de forma descrescente as frequências dos símbolos de um bloco recebidos pelo ficheiro do tipo freq
listFreq é o array das frequências e N o tamnanho desse array
index e posição são útilizados para percorrer o array das frequências
maximo guarda o maior valor encontrado à medida que se percorre o array das frequências
*/
void sort_list (int listFreq[], int N);


/**
\brief Funçao que calcula a melhor dividisao para o array das frequências, devolvendo o index onde se deve dividir
listFreq é o array das frequências
min é menor index no array das frequências 
max é o maior index no array das frequências 
total é o resultada da soma das frequencias do array passado
mindif é a variavel que guarda a menor diferença entre os grupos 
dif é a variavel que guarda a diferença entre os grupos
div é index que percorre o array
g1 é o grupo 1 das frequencias
*/
int calculate_best_divider (int listFreq[], int min, int max);


/**
\brief Função que calcula os códigos Shannon-Fano, dando uma lista de frequências    
listFreq é o array das frequências
codes é o array onde são guardados os códigos Shannon-Fano
start é o index utilizado para dizer onde começa o array das lista de frequências
end é o index utilizado para dizer onde acaba o array das lista de frequências
div é o index do local correspondente à melhor divisao do array da lista de frequências
*/
void calculate_codes_SF(int listFreq[], unsigned char *codes[],int start, int end);


/**
\brief Função que devolve o conteúdo entre dois "@", guardando-o na variavél counter
index é a variavél que percorre o buffer
freq_value é um array que guarda o conteudo entre os dois "@", colocando depois no counter
index_freq é um index que utilizado para percorrer o array freq_value
*/
void count_char_arroba(char *buffer, int *counter, int *index);


/**
\brief Função que devolve o conteúdo entre dois ";", guardando-o na variavél counter
index é a variavél que percorre o buffer
freq_value é um array que guarda o conteudo entre os dois ";", colocando depois no counter
index_freq é um index que utilizado para percorrer o array freq_value
*/
void count_char_semicolon(char *buffer, int *counter, int *index);


/**
\brief Função que devolve o conteúdo entre o "@" e o ";", ou seja, devolve o primeiro valor da frequência, guardando-o na variavél counter
index é a variavél que percorre o buffer
freq_value é um array que guarda o conteudo entre o "@" e o ";", colocando depois no counter
index_freq é um index que utilizado para percorrer o array freq_value
*/
void count_char_first_semicolon(char *buffer, int *counter, int *index);


/**
\brief Função que devolve o conteúdo entre o ";" e o "@", ou seja, devolve o último valor da frequência, guardando-o na variavél counter
index é a variavél que percorre o buffer
freq_value é um array que guarda o conteudo entre o ";" e o "@", colocando depois no counter
index_freq é um index que utilizado para percorrer o array freq_value
*/
void count_char_last_semicolon(char *buffer, int *counter, int *index);


/**
\brief Função que recebe o ficheiro .freq e devolve o ficheiro .cod
listFreq é o array utilizado para guardar as frequências
buffer guarda a informação do ficheiro recebido
codes é o array onde são guardados os códigos Shannon-Fano
tam[nr_blocos] é o array que guarda o tamanho de cada bloco
frequencia é a variavél que guarda a frequência atual
freq_anterior é a variável que guarda a frequência anterior
*/
void encode (char *freq);


#endif //MODULO_B_H
