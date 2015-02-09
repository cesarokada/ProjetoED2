/***************************************************************************
O arquivo arqFuncs.c vai conter as fun��es que envolvem abertura de arquivos
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FILE* posicionaCabeca(FILE *fp,int **head)
{
    char pos[10];

    rewind(fp);
    fread(pos,sizeof(char),4,fp);
    rewind(fp);
    fseek(fp,sizeof(int),0);

    return fp;
}

FILE *fileOpen(char *fileName, int *head) {//Abre os arquivos e l� o cabe�alho dos arquivos, retornando
                                           // atrav�s de passagem por parametro o inicio da lista de posi��es dispon�veis
    FILE *fp;//Ponteiro do arquivo aberto
    char valorCab[5];//String para armazenar o valor do cabe�alho
    int size,aux;

    if((fp = fopen(fileName,"r+b")) == NULL){//Abre arquivo no modo leitura/escrita bin�ria e caso ocorra erro...
        fclose(fp);//fecha o arquivo
        if((fp = fopen(fileName,"w+b")) == NULL)//E tenta abri-lo novamente...
            printf("ERRO AO ABRIR O ARQUIVO\n");//E escrever o erro
    }

    fseek(fp,0,SEEK_END);//Envia o ponteiro para o fim do arquivo...
    size = ftell(fp);//E calcula o seu tamanho em bytes...
    rewind(fp);//Retorna o ponteiro ao inicio do arquivo

    if (size) {//Caso o arquivo tenha conteudo
            fread(valorCab,sizeof(int),1,fp);//Ele l� um inicio da lista posi��es disponiveis no cabe�alho
                                             //e o armazena na variavel valorCab
            aux = atoi(valorCab);//Converte esse valor para inteiro
            if (aux != 0)//E caso a lista n�o esteja vazia
                *head = atoi(valorCab);//Seu valor � retornado no ponteiro passado por par�metro
    } else {//Caso o arquivo esteja vazio
        itoa(*head,valorCab,10);//O valor -1 � passado por parametro � convertido para string...
        fwrite(valorCab,4*sizeof(char),1,fp);//E � gravado no cabe�alho do arquivo,
                                //indicando que a lista de posi��es est� vazia
        fclose(fp);//O arquivo � fechado
        fp = fopen(fileName, "r+b");//E aberto novamente
    }

    return fp;//Retorna um ponteiro com o arquivo de dados aberto
}

FILE* fileOpenIdx(char *fileName, int *head) {//Abre um arquivo indice, retornando um ponteiro para o mesmo e um flag
                                              // indicando se ele foi fechado corretamente
    FILE *fp;//ponteiro para o arquivo de indice que ser� aberto
    char valorFlag[5];
    int size, aux;

    if((fp = fopen(fileName,"r+b")) == NULL){//abre o arquivo no modo de leitura/escrita binaria e em caso de erro...
        fclose(fp);// Fecha o aquivo...

        if((fp = fopen(fileName,"w+b")) == NULL)//E tenta abri-lo novamente no mode de leitura/escrita para...
            printf("ERRO AO ABRIR O ARQUIVO\n");//Adicionar linha de erro
    }

    fseek(fp,0,SEEK_END); //Posiciona o ponteiro no fim do arquivo
    size = ftell(fp); //Armazena o tamanho do arquivo
    rewind(fp); //Retorna ao inicio do arquivo

    if (size) { //Se o arquivo possuir algo escrito nele
        fread(valorFlag,sizeof(int),1,fp); //L� o flag no header do arquivo
        aux = atoi(valorFlag);//converte o flag para inteiro

        if (aux != 0)//Se o flag for diferente de zero...
            *head = atoi(valorFlag);//Seu valor � armazenado na variagel head e seu valor � retornado

    } else{//Se arquivo estiver vazio
        itoa(*head,valorFlag,10);//retorna o valor 0 indicando que h� erro
        fwrite(valorFlag,sizeof(int),1,fp);//E imprime o valor no cabe�alho do arquivo
        //fclose(fp);
        //printf("teste");
        //fp = fopen(fileName,"r+b");
        //fread(valorFlag,sizeof(int),1,fp);
        //printf("arquivo %d",atoi(valorFlag));
    }

    return fp;//Retorna o ponteiro do arquivo aberto
}
