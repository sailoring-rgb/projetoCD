#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "fsize.h"
#include "moduloC.h"
#define BUFFSIZE 100


//Reset a um array para poder ser re-utilizado
void reset_array(char *reset, int tamanho){

    for(int i = 0; i < tamanho; i++){
        reset[i] = '\0';
    }
}


//Determina qual � o maior numero de um array. Utilizado para determinar o maior bloco.
int maior_numero(int *tamanhos, int n_blocos){

    int maior = tamanhos[0];
    int index = 0;

    while(index < n_blocos){
        if(maior < tamanhos[index]){
            maior = tamanhos[index];
            index++;
        }
        else{
            index++;
        }
    }
    return maior;
}


//Fun��o para tornar um array num inteiro;
int calcula_array(char *numeros_string, int contador){

    int resultado = 0;
    int var = contador;

    for(int i = 0; i < var;i++){
        contador--;
        resultado = resultado + pow(10.0,(double)contador)*(numeros_string[i] -'0');
    }
    return resultado;
}


//Fun��o para calcular os tamanhos dos blocos;
//Recebe apontador para o array de caracteres do ficheiro ".cod" + n�mero de blocos + apontador para array dos tamanhos.
void tamanho_blocos(char *arrayCod, int n_blocos, int *tamanhos){

    int cont_posicao = 4;                                                               //Contador para percorrer o array. Ignora as primeiras 4 casas pois n�o interessam.
    int contador_at = 0;                                                                //Contador de @
    int index_temp = 0;                                                                 //Index do array temporario.
    int index_t_blocos = 0;                                                             //Posi�ao no array onde vai ser guardado o tamanho do bloco.
    char temporario[BUFFSIZE];

    while(arrayCod[cont_posicao] != '@'){                                               //Procura o primeiro @
        cont_posicao++;
    }

    while(contador_at < 2*n_blocos){                                                    //O tamanho do bloco encontra-se entre 2 @. Enquanto o contador de @ n�o for >= 2*n�mero de blocos
        if(arrayCod[cont_posicao] == '@'){                                                 //continua a procurar o tamanho do proximo bloco tamanho do proximo bloco.
            contador_at++;
            if(contador_at%2 == 0){                                                     //enquanto o contador de @ for par apenas passa posi�ao do arary a frente.
                cont_posicao++;
            }else{                                                                      //quando o contador de @ for impar guarda os valores lidos num array para depois converter em inteiro
                cont_posicao++;
                while(arrayCod[cont_posicao] != '@'){
                    temporario[index_temp] = arrayCod[cont_posicao];
                    ++index_temp;
                    ++cont_posicao;
                }
                tamanhos[index_t_blocos] = calcula_array(temporario,index_temp);        //converte os valores guardados no array temporario em inteiro e guarda num array.
                reset_array(temporario,100);
                index_temp = 0;
                index_t_blocos++;
                }
        }else{
            cont_posicao++;
        }
    }
}


//Fun��o para armazenar apenas o conteudo do bloco em array.
//Recebe apontador para o array de caracteres do ficheiro ".cod" + o index onde queremos come�ar + apontador para o array de caracteres do bloco
int array_bloco (char *arrayCod, int index_Cod, char *bloco_Cod){

    int index_bloco = 0;

    while(arrayCod[index_Cod] != '@'){                  //enquanto n�o for @ vai guardar o valor no arary bloco_cod
        bloco_Cod[index_bloco] = arrayCod[index_Cod];
        index_Cod++;
        index_bloco++;
    }
    return index_bloco;
}


//Fun��o para substituir uma Matriz
//Vai usar *bloco_Ori e a posicao (do valor binario no *bloco_Cod) para preencher a matriz conforme o seu valor.
void substitui_matriz(char *bloco_Ori, int t_bloco_atual, char posicao, char *valor_bin, char **matriz){

    for(int i  = 0; i < t_bloco_atual; i++){
        if(bloco_Ori[i] == posicao){
            strcpy(matriz[i], valor_bin);
        }
    }
}


//Fun��o para calcular quantos zeros ser�o preciso adicionar ao *str de modo ao str_cnt ser divisivel por 8;
int quantos_zeros( unsigned char *str, int str_cnt){

    if(str_cnt%8 != 0){
        int temp = str_cnt/8 + 1;
        temp = temp * 8 - str_cnt;
        return temp;
    }
    else{
        return 0;
    }
}


// Fun��o para imprimir o codigo codificado no ficheiro.
// Recebe o array e o file pointer onde vai ser imprimido + tamanho.
// Vamos agrupar o array dos binarios (*str) em grupos de 8 para converter para decimal.
void imprime_no_ficheiro (unsigned char *str , int str_cnt, FILE *fpShaf){

    char oito_char[8];                                      //o array vai receber 8 caracteres de '0' a '1';
    int contador, decimal = 0;

    fprintf(fpShaf,"%c%d%c",'@',str_cnt/8,'@');             //imprime o tamanho do bloco depois da compress�o;

    for(int i = 0; i < str_cnt/8; i++){
        for(int d = 0;d<8;d++){
            oito_char[d] = str[contador];
            contador++;
        }
        decimal = (int) strtol(oito_char, NULL, 2);         //Os caracteres que constituem o oito_char s�o '0' ou '1' formando n�mero binario;
        fputc(decimal, fpShaf);                              //O strtol converte esse n�mero binario em decimal;
                                                            //fputc imprime no ficheiro o char correspondente ao decimal;
    }
}


//Fun��o para criar uma string a partir de uma matriz de caracteres
int matriz_to_string(char **matriz, int t_bloco_atual, FILE *fpShaf){

    unsigned char *str;                                                             // string onde vai ficar a sequencia binaria;
    int memory_size, str_cnt = 0;                                                   // contador para o tamanho da memoria que vamos precisar de alocar | contador do tamanho da string;
    str = ( unsigned char*)malloc(sizeof( unsigned char));                          // como vamos inserir um caracter de cada vez, iniciamos a str com size de um caracter;


    for(int i = 0; i < t_bloco_atual; i++){
        // re-allocate (resize) memory for character read to be stored
        int realloc_size = (int)strlen(matriz[i]);                                  //Quantidade de memoria que vamos precisar de alocar para o array na posi�ao i da matriz;
        memory_size = memory_size + realloc_size;                                   //Para um realloc precisamos da memoria total que queremos alocar, ent�o somamos a nova memoria � que ja tinhamo;
        str = ( unsigned char*)realloc(str,  memory_size * sizeof( unsigned char));
        for(int x=0; x<strlen((matriz[i])); x++) {                                  //O ciclo for repete tantas vezes quanto o tamanho do array que esta na posi�ao i da matriz;
            str[str_cnt] = matriz[i][x];                                            //Copiar o array que esta na posi�ao i para o str;
            str_cnt++;
        }
    }

    int numero_zeros = quantos_zeros(str,str_cnt);                                  //fun��o quantos_zeros;
    int realloc_size = numero_zeros;                                                //temos de reallocar memoria para os '0' que vamos ter de adicionar;
    memory_size = memory_size + realloc_size;
    str = ( unsigned char*)realloc(str, memory_size * sizeof( unsigned char));

    for(int d = 0; d < numero_zeros; d++){                                          //ciclo para adicionar os 0 ao final da string;
        str[str_cnt] = '0';
        str_cnt++;
    }
    str[str_cnt] = '\0';
    imprime_no_ficheiro(str, str_cnt, fpShaf);                                      //fun�ao imprime_no_ficheiro;
    free(str);
    return str_cnt;
}


//Fun�ao para fazer a convers�o
int conversao(char *bloco_Cod, char *bloco_Ori, int t_bloco_atual, FILE *fpShaf, int maior_bloco){

    int index_temp = 0;
    int index_Cod = 0;
    int posicao = 0;
    int tamanho_string;
    char temporario[BUFFSIZE];

    char **matriz = (char **)malloc(maior_bloco * sizeof(char *));   //alocar a matriz
    for(int i=0; i<maior_bloco; i++)
        matriz[i] = (char *)malloc(BUFFSIZE * sizeof(char));

    for(int i = 0; i < maior_bloco; i++){                       //reset a matriz
        strcpy(matriz[i],"\0");
    }

    while(bloco_Cod[index_Cod] != '\0'){                        //vai percorrer o *bloco_Cod ignorando tudo que seja ';'
        if(bloco_Cod[index_Cod] == ';'){                        //guardando no *temporario um valor binario que encontre;
            posicao++;
            index_Cod++;
        }
        else{
            while(bloco_Cod[index_Cod] != ';'){
                temporario[index_temp] = bloco_Cod[index_Cod];
                index_Cod++;
                index_temp++;
            }
            temporario[index_temp] = '\0';

            substitui_matriz(bloco_Ori, t_bloco_atual, posicao, temporario, matriz);
            reset_array(temporario, 100);
            index_temp = 0;
        }
    }
    tamanho_string = matriz_to_string( matriz, t_bloco_atual, fpShaf);

    for(int i = 0; i < maior_bloco; i++){
        free(matriz[i]);
    }
    return tamanho_string;
}


//Fun��o vai escrever os blocos dos ficheiros nos *bloco_Cod e *bloco_Ori
void escreve_bloco (char *arrayOri, char *arrayCod, int n_blocos, int *tamanhos, int maior_bloco, FILE *fpShaf, int *tamanho_blocos_comprimidos){

    int contador_at = 0;
    int cont_posicao = 4;
    int index_array_ori = 0;
    char *bloco_Cod;
    bloco_Cod = (char*)malloc(2500*sizeof(char));
    char *bloco_Ori;
    bloco_Ori = (char*)malloc(maior_bloco*sizeof(char));

    reset_array(bloco_Ori, maior_bloco);
    reset_array(bloco_Cod, 2500);

    while (arrayCod[cont_posicao] != '@'){
        cont_posicao++;
    }

    for(int i = 0; i < n_blocos; i++){
        while(contador_at < 2){
            if(arrayCod[cont_posicao] == '@'){
                contador_at++;
                cont_posicao++;
            }
            else{
                cont_posicao++;
            }
        }
        contador_at = 0;
        cont_posicao = array_bloco(arrayCod, cont_posicao, bloco_Cod);
        int d = 0;
        for(d; d < tamanhos[i];d++){
            bloco_Ori[d] = arrayOri[index_array_ori];
            index_array_ori++;
        }
        tamanho_blocos_comprimidos[i] = conversao(bloco_Cod, bloco_Ori, tamanhos[i], fpShaf, maior_bloco);
    }
    free(bloco_Ori);
}


int blocos(char *array){

    int nblocos = 0;
    char temporario[BUFFSIZE];
    int posicao = 3;
    int index = 0;

    while(array[posicao] != '@'){
        temporario[index] = array[posicao];
        ++index;
        ++posicao;
    }
    nblocos = calcula_array(temporario, index);
    return nblocos;
}


//Copia uma string para outra
void copy_string(char *NomeFicheiro1, char *NomeFicheiro2){
int i=0;
    while(NomeFicheiro1[i] != '\0'){
        NomeFicheiro2[i] = NomeFicheiro1[i];
        i++;
    }
}


//Escreve num array todos os caracteres do ficheiro
void escreve_array(FILE *fp, char *array, int size){
    unsigned char c;
    for(int i = 0; i < size; i++){
        c = fgetc(fp);
        array[i] = c;
    }
}


//Imprime no final do ficheiro taxa de compress�o de cada bloco.
void print_blocos(int n_blocos, int *tamanhos,  int *tamanho_comprimido){
    for (int i = 0; i < n_blocos; i++){
        if(tamanho_comprimido[i]%8 == 0){
            printf("Tamanho antes/depois & taxa de compress�o (bloco %d): %d / %d \n", i+1, tamanhos[i], tamanho_comprimido[i]/8);
        }
        else{
            printf("Tamanho antes/depois & taxa de compressao (bloco %d): %d / %d \n", i+1, tamanhos[i], tamanho_comprimido[i]/8+1);
        }
    }
}


//Calcula tamanho do ficheiro
int fsizee(char *ficheiro){
    unsigned long long total;
    long long n_blocks;
    unsigned long size_of_last_block, block_size;

    block_size = FSIZE_DEFAULT_BLOCK_SIZE;
    n_blocks = fsize(NULL, ficheiro, &block_size, &size_of_last_block);
    total = (n_blocks-1) * block_size + size_of_last_block;
    return total;
}


//Fun��o para calcular a taxa total de compressao do ficheiro.
float taxa_compressao (int n_blocos, int *tamanho_blocos_comprimidos, int sizeOri){
    float tamanho_final;
    int somatorio_tamanhos_comprimido = 0;
    for(int i = 0; i < n_blocos;i++){
        if(tamanho_blocos_comprimidos[i]%8 == 0){
            somatorio_tamanhos_comprimido = somatorio_tamanhos_comprimido + (tamanho_blocos_comprimidos[i]/8);
        }
        else{
            somatorio_tamanhos_comprimido = somatorio_tamanhos_comprimido + (tamanho_blocos_comprimidos[i]/8)+1;
        }
    }
    tamanho_final = (float)somatorio_tamanhos_comprimido/sizeOri*100;
    return tamanho_final;
}


//Fun�ao principal onde se inicializa o temporizador, l� e criam ficheiros e se imprimem as informa��es finais.
void fPrincipal(char *nomeFicheiroOriginal)
{
    clock_t Ticks[2]; //numero de ticks de relogio, 0 para come�ar e 1 para acabar.
    int n_blocos = 0;
    int sizeOri = 0;
    int sizeCod = 0;
    int maior_bloco = 0;
    char nomeFicheiroCod[BUFFSIZE] = {0};
    char nomeFicheiroGerado[BUFFSIZE] = {0};
    //char nomeFicheiroOriginal[BUFFSIZE] = {0};

//Defini��o de vari�veis
    setbuf(stdout, 0);
    //scanf("%s",&nomeFicheiroOriginal);
	copy_string(nomeFicheiroOriginal, nomeFicheiroCod);
    strcat(nomeFicheiroCod,".cod");


//INICIALiZAR O TEMPORIZADOR
	Ticks[0] = clock();

//Preparar os ficheiros e temporizador
    sizeOri = fsizee(nomeFicheiroOriginal);
    unsigned char *arrayOri;
    arrayOri = (unsigned char*)malloc(sizeOri * sizeof(unsigned char));
    sizeCod = fsizee(nomeFicheiroCod);
    unsigned char *arrayCod;
    arrayCod = (unsigned char*)malloc(sizeCod * sizeof(unsigned char));


//Inicializar os arrays com o tamanho certo
    FILE *fPointerOri;
    fPointerOri = fopen(nomeFicheiroOriginal, "rb");
	escreve_array(fPointerOri, arrayOri, sizeOri);
    fclose(fPointerOri);

//Percorre o ficheiro original e escreve-o no array "arrayOri"
    FILE *fPointerCod;
    fPointerCod = fopen(nomeFicheiroCod, "rb");
    escreve_array(fPointerCod, arrayCod, sizeCod);
    fclose(fPointerCod);


//Percorre o ficheiro .cod e escreve-o no array "arrayCod"
    n_blocos = blocos(arrayCod);
    int tamanhos[n_blocos];
    int tamanho_blocos_comprimidos[n_blocos];
    tamanho_blocos(arrayCod, n_blocos,tamanhos);
    maior_bloco = maior_numero(tamanhos, n_blocos);


//Cria o nome do ficheiro a ser gravado
    copy_string(nomeFicheiroOriginal,nomeFicheiroGerado);
    strcat(nomeFicheiroGerado,".shaf");

//Escreve no ficheiro a ser gravado
    FILE *fpShaf;
    fpShaf = fopen(nomeFicheiroGerado, "wb");
    fprintf(fpShaf,"%c%d",'@',n_blocos);
    escreve_bloco(arrayOri, arrayCod, n_blocos, tamanhos, maior_bloco, fpShaf, tamanho_blocos_comprimidos);
    free(arrayOri);
    free(arrayCod);
    fclose(fpShaf);

    float tamanho_final = taxa_compressao(n_blocos, tamanho_blocos_comprimidos, sizeOri);

//FINALIZAR TEMPORIZADOR
    Ticks[1] = clock();
    double Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC; //calcula o tempo de execu��o em milissegundos.

//INFORMACOES FINAIS
    printf("\n");
    printf("Joel Araujo, A76603, Diogo Rebelo, A93180, MIEI/CD, DATA \n");
    printf("Modulo: c (codificacao dum ficheiro de simbolos)\n");
    printf("Numero de blocos: %d \n", n_blocos);
    print_blocos(n_blocos, tamanhos, tamanho_blocos_comprimidos);
    printf("Taxa de compressao global: %.2f%% \n", tamanho_final);
    printf("Tempo de execucao do modulo (milissegundos): %g ms \n", Tempo);
    printf("Ficheiros gerados: %s \n", nomeFicheiroGerado);
}

