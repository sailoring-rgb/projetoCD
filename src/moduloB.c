#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "moduloB.h"

#define MAXCHAR 140000
#define PALAVRAS 257


void swap(int array[], int x, int y) { 
    int aux = array[x]; 
    array[x] = array[y]; 
    array[y] = aux; 
}


int add(int listFreq[],int min, int max){
    int index, result = 0;
    for(index = min; index <= max; index++){
        result += listFreq[index];
    }
    return result;
}


void add_bit_to_code(unsigned char *bit, unsigned char *codes[], int start, int end){
    while(start<=end){
         strcat(codes[start],bit);
         start++;
    }
}


void sort_list (int listFreq[], int N) {
    int index, max, position;
    for (index = 0; index < N-1; index ++){
        max= index; 
        for (position = index+1; position < N; position++)
            if (listFreq[max] < listFreq[position]) max = position;
        if (index != max) swap (listFreq, index, max);
	}
}


int calculate_best_divider (int listFreq[], int min, int max){
   int div=min, total, mindif, dif, g1=0;
   total = mindif = dif = add(listFreq,min,max);
   while(dif==mindif){  
       g1 = g1 + listFreq[div];
       dif = abs(2*g1 - total); 
       if (dif<mindif){
            div = div+1;
            mindif = dif;
       }
       else dif = mindif+1;
      }
   return div-1;
}


void calculate_codes_SF(int listFreq[], unsigned char *codes[],int start, int end){
     int div;
     if(start!=end){
        div=calculate_best_divider(listFreq,start,end);
        add_bit_to_code("0",codes,start,div);
        add_bit_to_code("1",codes,div+1,end);
        calculate_codes_SF(listFreq,codes,start,div);
        calculate_codes_SF(listFreq,codes,div+1,end);
     }
     
}


void count_char_arroba(char *buffer, int *counter, int *index){
    char freq_value[9];
    int index_freq, splitter;
    for(splitter=0; splitter<2;(*index)++){
        if (splitter==1){
            for(index_freq=0; buffer[*index]!=64;(*index)++,index_freq++){
                freq_value[index_freq]=buffer[*index];
            }
        }
        if (buffer[*index]==64) splitter++;    
    }
    (*index)--;
    if(atoi(freq_value)==0) (*counter)=freq_value[0];
    else *counter=atoi(freq_value);
    memset(freq_value,0,9);
}


void count_char_semicolon(char *buffer, int *counter, int *index){
    char freq_value[9];
    int index_freq, splitter;
    for(splitter=0; splitter<2;(*index)++){
        if (splitter==1){
            for(index_freq=0; buffer[*index]!=59;(*index)++,index_freq++){
                freq_value[index_freq]=buffer[*index];
            }
        }
        if (buffer[*index]==59) splitter++;    
    }
    (*index)--;
    if(atoi(freq_value)==0) (*counter)=freq_value[0];
    else *counter=atoi(freq_value);
    memset(freq_value,0,9);
}


void count_char_first_semicolon(char *buffer, int *counter, int *index){
    char freq_value[9];
    int index_freq, splitter;

    for(index_freq=0; buffer[*index]!=59;(*index)++,index_freq++){
        freq_value[index_freq]=buffer[*index];
    }

    (*index)--;
    if(atoi(freq_value)==0) (*counter)=freq_value[0];
    else *counter=atoi(freq_value);
    memset(freq_value,0,9);
}


void count_char_last_semicolon(char *buffer, int *counter, int *index){
    char freq_value[9];
    int index_freq, splitter;
    for(index_freq=0; buffer[*index]!=64;(*index)++,index_freq++){
        freq_value[index_freq]=buffer[*index];
    }
    (*index)--;
    if(atoi(freq_value)==0) (*counter)=freq_value[0];
    else *counter=atoi(freq_value);
    memset(freq_value,0,9);
}


void encode (char *freq){

    // Inicio da contagem 
    clock_t Ticks[2];
    Ticks[0] = clock();

    int listFreq[PALAVRAS]; 
    unsigned char buffer[MAXCHAR];
    unsigned char *codes[256];
    char c[10]; 
    for(int i=0; i<256; i++){
        codes[i]=(unsigned char*)malloc(sizeof(unsigned char*)*257);
        *(codes[i])=0;
    }   

    FILE *fileFreq;
    fileFreq = fopen(freq, "rb"); 
    if (freq == NULL) printf ("Error opening freq file");
    fgets(buffer, MAXCHAR, fileFreq);
    fclose(fileFreq);
    FILE *fileCod;
    fileCod=fopen("file.cod","aw+");
    
    int index = 0,nr_blocos,counter ;
    count_char_arroba(buffer,&counter,&index); // Guarda o tipo do FREQ na variável counter
    count_char_arroba(buffer,&nr_blocos,&index); // Guarda o nº de blocos na variável nr_blocos
    
    int nrblocos=10, size=10;
    nrblocos=sprintf(c,"%d",nr_blocos);
    // Escreve no ficheiro .cod a informação inicial, isto é, os arrobas, o tipo do FREQ e o nº de blocos
    fprintf(fileCod, "%c%c%c%s%c", '@', counter , '@', c, '@'); 

    int tam[nr_blocos];
    int frequencia;
    int freq_ant = 0;

    // O ciclo vai guardar as frequências na listFreq e depois calcular os códigos SF das mesmas, escrevendo-os no ficheiro .cod
    for (int bloco_atual=0; bloco_atual<nr_blocos; bloco_atual++){
        count_char_arroba(buffer,&tam[bloco_atual],&index); // Guarda o tamanho bloco atual no array tam[bloco_atual]
        index++;
        count_char_first_semicolon(buffer,&frequencia,&index); // Guarda a primeira frequência do bloco atual na variável frequencia 
        freq_ant = frequencia;
        listFreq[0] = freq_ant;
        for (int a=1; a<255; a++){
            count_char_semicolon(buffer,&frequencia,&index); // Guarda a frequência entre dois pontos e vírgulas na variável frequencia
            if(frequencia==0){
                listFreq[a]=freq_ant;
            }else{
                if(frequencia!=0){
                    freq_ant = frequencia;
                    listFreq[a]=frequencia;
                }
            }
        
        }
        index++;
        count_char_last_semicolon(buffer,&frequencia,&index); // Guarda a última frequência do bloco atual na variável frequencia
        if(frequencia==0){
                listFreq[255]=freq_ant;
        }else{
            if(frequencia!=0){
                freq_ant = frequencia;
                listFreq[255]=frequencia;
            }
        }
        sort_list(listFreq,256); // Ordena a lista de frequências de forma descrescente
        calculate_codes_SF(listFreq,codes,0,255); // Calcula os códigos SF para cada frequência
        size=sprintf(c,"%d",tam[bloco_atual]);
        fprintf(fileCod, "%s%c",c,'@'); // Escreve no ficheiro .cod um arroba
        for(int i=0; i<256;i++){
            fprintf(fileCod,"%s%c",codes[i],';'); // Escreve no ficheiro .cod os codígos SF das frequências
        }
        fprintf(fileCod,"%c",'@'); // Escreve no ficheiro .cod um arroba
    }
    fprintf(fileCod,"%c",'0'); // Escreve no ficheiro .cod o zero
    fclose(fileCod);

    // Fim da contagem
    Ticks[1] = clock();

    // Cálculo do tempo
    double time = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;

    printf("Ana Filipa, a93284, Ana Paula, a93268, MIEI/CD, 1-jan-2021\n");
    printf("Modulo: t (calculo dos codigos dos simbolos)\n");
    printf("Numero de blocos: %d\n",nr_blocos);
    printf("tam dos blocos analisados no ficheiro de simbolos: ");
    for(int i=0;i<nr_blocos;i++){
        printf("%d", tam[i]);
        if (i<nr_blocos-1)
            printf("%c", '/');
    }
    printf(" bytes\n");
    printf("Tempo de execucao do modulo (milissegundos): %g\n",time);
    printf("Ficheiro gerado: file.cod \n");
}




