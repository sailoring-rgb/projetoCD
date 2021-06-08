#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "moduloD.h"
#define FILENAME 100
#define TAMFC 10000000
#define TAMBLOCO 200000

char* filenameChange (char *base,char *changethis,char *forthis,char *returnme){
    strcpy(returnme, base);
    char *i;
    i= strstr(returnme,changethis);
    if(i==NULL) return i;
    strcpy(i, forthis);
    return i;
}

ABin newNodo () {
    ABin t;
    t = malloc (sizeof (struct nodoarv));
    t->zero = NULL ; t->um = NULL;
    t->code=-1;
    return t;
}


void addOrd (ABin *a, char *x,int i,int tam,int codigo){
    if((*a)==NULL) (*a) = newNodo();
    if (i==tam) (*a)->code=codigo;
    else if (x[i]=='0') addOrd(&((*a)->zero),x,i+1,tam,codigo);
    else addOrd(&((*a)->um),x,i+1,tam,codigo);
}


int procura (ABin *a, char *x,int i,int tam){
    if((*a)==NULL) {
        printf("Erro!!!");
        return -1;
    }
    if (i==tam) return (*a)->code;
    else if (x[i]=='0') return procura(&((*a)->zero),x,i+1,tam);
    else return procura(&((*a)->um),x,i+1,tam);
}


void rodaIndice(char *arr,int indice){
    char tmp;

    tmp=arr[indice];
    for (;indice>0;indice--) {
        arr[indice] = arr[indice-1];
    }
    arr[indice]=tmp;
}


void contachars(unsigned char *buff,int *counter,int *i){
    char charcounter[10];

    for(int j=0;j<10;j++) charcounter[j]='a';
    for (int cont=0; cont<2;(*i)++){
        if(cont==1){
            for(int j=0;buff[*i]!=64;(*i)++,j++){
                charcounter[j]=buff[*i];
            }
        }
        if(buff[*i]==64) cont++;
    }
    (*i)--;
    if(atoi(charcounter)==0) (*counter) = charcounter[0];
    else *counter= atoi(charcounter);
    memset(charcounter,0,10);
}


char contaFILEchars(FILE *file,int *counter,char c){
    char charcounter[10];
    
    if(c!='@') c=fgetc(file);
    for(int i=0;i<10;i++) charcounter[i]='a';
    for (int cont=0; cont<2;){
        if(cont==1){
            for(int j=0;c!='@';j++){
                charcounter[j]=c;
                c=fgetc(file);
            }
        }
        if(c=='@') cont++;
        if (cont==1) {
            c=fgetc(file);
        }
    }
    if(atoi(charcounter)==0) (*counter) = charcounter[0];
    else *counter= atoi(charcounter);
    memset(charcounter,0,10);
    return c;
}


int convertdecbin(int dec) {
    int resto,bin=0,i=1;
    while (dec!=0) {
        resto=dec%2;
        dec/=2;
        bin+=resto*i;
        i*=10;
    }
    return bin;
}


int readFile(int *bytes,char *text,unsigned char **buff,int size){
    *buff = malloc(size * sizeof(unsigned char));
    FILE *file;
    file = fopen(text, "rb");
    if (file == NULL){
        printf ("Error opening file\n");
        return -1;
    }
    *bytes = fread ( *buff, 1, size, file);
    fclose(file);
    return 0;
}


int calculaRLETam(char *freq,int **tamblocoRLE,int *nblocos){
    unsigned char *buffRLE;
    int decodetype,i=0,bytes=0;
    if(readFile(&bytes,freq,&buffRLE,TAMFC)==-1) return -1;
    contachars(buffRLE,&decodetype,&i);
    contachars(buffRLE,&(*nblocos),&i);

    *tamblocoRLE = malloc((*nblocos) * sizeof(int));

    for(int blocoatual=0;blocoatual<(*nblocos);blocoatual++){
        contachars(buffRLE,&(*tamblocoRLE)[blocoatual],&i);
        i++;
        while (buffRLE[i]!='@') i++;
    }
    free(buffRLE);
    return 0;
}


void seqRLE(unsigned char *buff, int posbuff, unsigned char *output, int *posout){

    unsigned char simb; //char que vai guardar o símbolo que vai ser repetido
    unsigned char count; //contador para indicar o numero de vezes que o simb vai ser repetido
    
    simb = buff[posbuff];
    count = buff[posbuff+1];

    for (int j=0;j<count;j++){
        output[*posout] = simb;
        (*posout)++;
    }
}


int decodeBlocoRLE (unsigned char *output,unsigned char *buff, int tamanhoB) {

    int i,j=0;
    unsigned char simb;

    for (i = 0; i < tamanhoB; i++) {
        simb=buff[i];
        if (simb == 0) { //verificar se aparece 00, ou seja, se se necessita de ir para a seqRLE
            i++; //faz com que o i passado na seqRLE seja do valor correto, passando à frente o 00
            seqRLE(buff, i, output, &j);
            i++;//para avançar os 2 valores que definiram o simb e o count na seqRLE
        } else {
            output[j] = simb;
            j++;
        }
    }
    return j;
}


int decodeCOD (char *cod,int *nblocos,int **tamanhoF,ABin **tree){
    unsigned char *buff1; //Guarda a informação do ficheiro cod
    char word[270]; //Guarda os codigos binarios lidos do ficheiro cod ate serem escritos na tree
    int bytes=0; //Bytes lidos do ficheiro COD
    int i=0; //Apontador do buff1 para leitura do COD
    int decodetype; //Indica que tipo de ficheiro COD estamos a ler (rle ou shaf) ou seja R ou N respetivamente.

    if(readFile(&bytes,cod,&buff1,TAMFC)==-1) return -1; //Leitura do ficheiro cod.
    contachars(buff1,&decodetype,&i); //Avança o I e descobre o tipo do COD
    contachars(buff1,nblocos,&i); //Avança o I e descobre o numero de blocos
    *tree=malloc((*nblocos)*sizeof(ABin)); //Cria um array de nblocos de arvores
    for(int j=0;j<*nblocos;j++) (*tree)[j] = NULL; //Coloca as arvores todas vazias
    *tamanhoF = malloc((*nblocos) * sizeof(int)); //Cria um array de nblocos onde se vai guardar o tamanho final do bloco
    for (int blocoatual =0,tamWord; blocoatual<(*nblocos); blocoatual++){ //Faz o descrito neste for nbloco vezes
        contachars(buff1,&(*tamanhoF)[blocoatual],&i); //Guarda o tamanho final do bloco atual no array tamanhoF
        i++;
        for (int y=0; buff1[i] !='@'; i++,y++){ //O bloco acaba quando chegar ao char '@'
            for(tamWord=0;buff1[i]!=';'&&buff1[i]!='@';i++,tamWord++) { //Guarda a informacao entre os ';'
                word[tamWord] = buff1[i]; 
            }
            if(buff1[i]=='@') i--;
            if(tamWord!=0) addOrd (&(*tree)[blocoatual], word, 0, tamWord, y); //Se houve informacao guardada entao vai ser escrita na arvore
        }
    }
    free(buff1); //Liberta a memoria do malloc alocada no readFile
    return 0;
}


void decodeBLOCOSHAF (int tamBloco,ABin tree,unsigned char *final,int *buff3){
    char stack[8],value[256];

    for(int belongs=-1,tamstack=8,i=0,j=0;j<tamBloco;j++,belongs=-1){
        for(int tamvalue=0;belongs==-1;tamvalue++,tamstack++){
            if(tamstack==8){
                tamstack = sprintf(stack,"%d",buff3[i]);
                for(;tamstack!=8;tamstack++){
                    stack[tamstack]='0';
                    rodaIndice(stack,tamstack);
                }
                tamstack=0;
                i++;
            }
            value[tamvalue]=stack[tamstack];
            belongs=procura(&(tree),value,0,tamvalue+1);
        }
        final[j]=belongs;
    }
}


int decoderle (char *rle,char *freq,char *unzipedName,int nblocos,int **tamanhoI,int **tamanhoRLE, int info){
    unsigned char *buff4; //Bloco onde vai ser guardado bloco a bloco do ficheiro rle
    buff4=malloc(TAMFC*sizeof(unsigned char));
    unsigned char *output; //Bloco do ficheiro rle onde foi feita descompressao
    output=malloc(sizeof(unsigned char)*TAMFC*100);

    FILE *fileRle; //Abrir o ficheiro(decidi nao usar o readfile para ter maior manipulacao do ficheiro)
    fileRle = fopen(rle, "rb");
    if (fileRle == NULL) {
        printf ("Error opening rle file\n"); //Teste basico para ver se o ficheiro existe
        return -1;
    }

    FILE *finalFile;//Ficheiro onde vai ser escrito o resultado da descodificacao RLE
    finalFile = fopen(unzipedName, "wb");

    for (int blocoatual=0; blocoatual<nblocos; blocoatual++){
        fread ( buff4, 1,(*tamanhoI)[blocoatual], fileRle);
        decodeBlocoRLE (output,buff4,(*tamanhoRLE)[blocoatual]);
        fwrite(output , 1 , (*tamanhoRLE)[blocoatual] , finalFile );//Escrever o array output no ficheiro
    }
    
    fclose(finalFile);
    fclose(fileRle);
    return 0;
}

int decodeSHAF (char *shaf,char *unzipedName,int *nblocos,int **tamanhoI,int *tamanhoF,int *tamanhoRLE,ABin *tree,int info){
    int nblocosshaf,bytes;
    char apontador; //Ultimo char lido do file
    unsigned char *buff2; //Guarda a informação do ficheiro shaf(em blocos)
    buff2=malloc(TAMFC*sizeof(unsigned char));
    int *buff3; //Ficheiro shaf em binario (em blocos)
    buff3=malloc(TAMFC*sizeof(int));
    unsigned char *final; //Ficheiro final .txt ou txt.rle 
    final=malloc(TAMFC*10*sizeof(unsigned char));
    unsigned char *output;
    if(info==-3 || info==0) output=malloc(TAMFC*100*sizeof(unsigned char));
    
    *tamanhoI = malloc((*nblocos) * sizeof(int)); //Cria um array de nblocos onde se vai guardar o tamanho incial do bloco

    FILE *fileshaf; //Abrir o ficheiro(decidi nao usar o readfile para ter maior manipulacao do ficheiro)
    fileshaf = fopen(shaf, "rb");
    if (fileshaf == NULL){
        printf ("Error opening shaf file\n"); //Teste basico para ver se o ficheiro existe
        return -1;
    }

    apontador = contaFILEchars(fileshaf,&nblocosshaf,'a'); //Vai ler os primeiros chars do ficheiro.
    //E inicializado com 'a' porque ainda nao foi lido nenhum char
    //O apontador vai passar a ser '@' e ja vamos ter o valor do numero de blocos do ficheiro shaf
    if(nblocosshaf!=(*nblocos)){ //Verificação basica para confirmar se o ficheiro .shaf corresponde ao .cod lido
        printf("Ficheiro shaf nao corresponde ao ficheiro cod\n");
        return -1;
    }

    FILE *finalFile;//Ficheiro onde vai ser escrito o resultado da descodificacao
    finalFile = fopen(unzipedName, "wb");
                     
    for (int blocoatual=0; blocoatual<nblocosshaf; blocoatual++){
        apontador = contaFILEchars(fileshaf,&(*tamanhoI)[blocoatual],apontador);//Vai ler chars que indicam o tamanho do bloco inicial.
        //Essa informacao vai ser guardada no array tamanhoI
        //O apontador vai acabar em '@' ou seja os chars seguintes ja fazem parte da descodificao

        bytes = fread ( buff2, 1,(*tamanhoI)[blocoatual], fileshaf);//Colocar o bloco inteiro num array
        

        for(int z=0;z<bytes;z++){ //Transformar o array de chars num array de binarios
            buff3[z]=convertdecbin(buff2[z]);
        }

        decodeBLOCOSHAF (tamanhoF[blocoatual],tree[blocoatual],final,buff3); //O resultado da descodificacao SF vai ficar no array final
        if(info!=-1 && info!=-2)decodeBlocoRLE (output,final,tamanhoF[blocoatual]); //O resultado da descodificacao RLE vai ficar no array output
        if(info==-1 || info==-2)fwrite(final , 1 , tamanhoF[blocoatual] , finalFile ); //Escrita do array final (caso )
        else fwrite(output , 1 , tamanhoRLE[blocoatual] , finalFile ); //Escrita do array output
        apontador=fgetc(fileshaf); //Colocar o apontador em '@' de novo para se recomecar o ciclo
    }
    fclose(finalFile);
    fclose(fileshaf);
    return 0;
}


int decodesf (char *cod,char *shaf,char *unzipedName,int *nblocos,int **tamanhoI,int **tamanhoF,int **tamanhoRLE,int info){
    ABin *tree;
    if(decodeCOD (cod,nblocos,tamanhoF,&tree)==-1) return -1;
    if(decodeSHAF (shaf,unzipedName,nblocos,tamanhoI,*tamanhoF,*tamanhoRLE,tree,info)==-1) return -1;
    return 0;
}

int decode (char *cod,char *shaf,char *rle,char *freq,char *unzipedName,int info){
    int nblocos,*tamanhoI,*tamanhoF,*tamanhoRLE;
    
    //Inicio da contagem
    clock_t Ticks[2];
    Ticks[0] = clock();

    if (info==-3||info==0) if(calculaRLETam(freq,&tamanhoRLE,&nblocos)==-1) return 0;
    if (info==-3) if(calculaRLETam(cod,&tamanhoI,&nblocos)==-1) return 0;
    if (info!=-3&&info<=0){
         if(decodesf (cod,shaf,unzipedName,&nblocos,&tamanhoI,&tamanhoF,&tamanhoRLE,info)==-1) return -1;
    }
    else if(decoderle(rle,freq,unzipedName,nblocos,&tamanhoI,&tamanhoRLE,info)==-1)return 0;
    
    //Fim da contagem
    Ticks[1] = clock();

    //Calculo do tempo
    double tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;


    //Escrita na consola.
    printf("Luis Silva, a93293, Goncalo Carvalho, a93260, MIEI/CD , 1-jan-2021\n");
    printf("Modulo: d (descodificacao dum ficheiro shaf)\n");
    if(info==-3) printf("Numero de blocos: %d \n",nblocos);
    else printf("Numero de blocos: %d \n",nblocos);
    //Casos para os diferentes comandos em que não são usadas as variaveis todas.
    if(info==-1||info==-2) for(int i=0;i<nblocos;i++) printf("Tamanho antes/depois do ficheiro gerado (bloco %d): %d/%d\n",i+1,tamanhoI[i],tamanhoF[i]);
    else for(int i=0;i<nblocos;i++) printf("Tamanho antes/depois do ficheiro gerado (bloco %d): %d/%d\n",i+1,tamanhoI[i],tamanhoRLE[i]);
    
    printf("Tempo de execucao do modulo (milissegundos): %g\n",tempo);
    printf("Ficheiro gerado: ");
    for(int i=0;unzipedName[i];i++) printf("%c",unzipedName[i]);
    printf("\n");
    return 0;
}

int lecomandoD(int argc, char *argv[]){
    char cod[FILENAME]; 
    char unzipedName[FILENAME];
    char rle[FILENAME];
    char freq[FILENAME];
    int info=0;

    if(argc==5 &&(!strcmp(argv[4], "r")||!strcmp(argv[4], "-r"))){ //caso em que se passa o RLE como argumento
        if(strcmp(strstr(argv[1],".rle"),".rle")){ //verificar se é recebido um ficheiro RLE
            printf("Erro! Por favor ensira um ficheiro .rle\n");
            return -1;
        }
        filenameChange(argv[1],".rle",".freq",freq);
        filenameChange(argv[1],".rle",".rle.freq",cod);
        strcpy(rle,argv[1]);
        filenameChange(argv[1],".rle","",unzipedName);
        info=-3;
    }
    else { //caso em que se passa o SHAF como argumento
        if(strcmp(strstr(argv[1],".shaf"),".shaf")){ //verificar se é recebido um ficheiro SHAF
            printf("Erro! Por favor ensira um ficheiro .shaf\n");
            return -1;
        }
        if(argc==5 &&(!strcmp(argv[4], "s")||!strcmp(argv[4], "-s"))){ //verificar se foi passado o comando s
            filenameChange(argv[1],".shaf","",unzipedName);
            info = -2;
            printf("hi\n");
        }
        else if (argc>=5) return -1; //caso tenha 5 argumentos e nao seja e o ultimo não seja s entao não é um comando conhecido
        else if (filenameChange(argv[1],".rle.shaf","",unzipedName)==NULL){ //se for uma descompressao normal entao verificar se o ficheiro é RLE
            filenameChange(argv[1],".shaf","",unzipedName);
            info=-1;
        }
        filenameChange(argv[1],".shaf",".cod",cod);
        filenameChange(argv[1],".shaf",".rle",rle);
        filenameChange(argv[1],".rle.shaf",".freq",freq);
    }
    return decode(cod,argv[1],rle,freq,unzipedName,info);
}
