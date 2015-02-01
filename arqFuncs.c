/***************************************************************************
O arquivo arqFuncs.c vai conter as funções que envolvem abertura de arquivos
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

FILE *fileOpen(char *fileName, int *head)
{
    FILE *fp;
    char valorCab[5];
    int size,aux;

    if((fp = fopen(fileName,"r+b")) == NULL){
        fclose(fp);
        if((fp = fopen(fileName,"w+b")) == NULL)
            printf("ERRO AO ABRIR O ARQUIVO\n");
    }
    fseek(fp,0,SEEK_END);
    size = ftell(fp);
    rewind(fp);

    if (size){
            fread(valorCab,sizeof(int),1,fp);
            aux = atoi(valorCab);
            if (aux != 0)
                *head = atoi(valorCab);
    }
    else{
        itoa(*head,valorCab,10);
        fwrite(valorCab,4*sizeof(char),1,fp);
        fclose(fp);
        fp = fopen(fileName, "r+b");
    }

    return fp;
}

FILE* fileOpenIdx(char *fileName, int *head)
{
    FILE *fp;
    char valorFlag[5];
    int size,aux;

    if((fp = fopen(fileName,"r+b")) == NULL){
        fclose(fp);
        if((fp = fopen(fileName,"w+b")) == NULL)
            printf("ERRO AO ABRIR O ARQUIVO\n");
    }
    fseek(fp,0,SEEK_END);
    size = ftell(fp);
    rewind(fp);

    if (size){
        fread(valorFlag,sizeof(int),1,fp);
        aux = atoi(valorFlag);
        if (aux != 0)
            *head = atoi(valorFlag);
    }
    else{
        itoa(*head,valorFlag,10);
        fwrite(valorFlag,sizeof(int),1,fp);
        //fclose(fp);
        printf("teste");
        //fp = fopen(fileName,"r+b");
        //fread(valorFlag,sizeof(int),1,fp);
        //printf("arquivo %d",atoi(valorFlag));
    }

    return fp;
}
