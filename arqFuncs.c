/***************************************************************************
O arquivo arqFuncs.c vai conter as funções que envolvem abertura de arquivos
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*int numDigits (int n)
{
    int d = 0;

    if (n<0) {
        d = 1;
        n *= -1;
    }
    do {
        ++d;
        n /= 10;
    } while (n);
    return d;
}*/

FILE* posicionaCabeca(FILE *fp,int **head)
{
    char pos[10];

    rewind(fp);
    fread(pos,sizeof(char),4,fp);
    rewind(fp);
    fseek(fp,sizeof(int),0);
    //n = numDigits(**head);
    //fseek(fp,n*sizeof(char),0);
    /*if(!(strcmp(filename,FileAP2))){
        fread(&pos,sizeof(int),1,fp);
        headListAP2 = atoi(pos);
        rewind(fp);
        n = numDigits(headListAP2);
        fseek(fp,n*sizeof(char),0);
    }
    else{
        fread(&pos,sizeof(int),1,fp);
        headListAP1 = atoi(pos);
        rewind(fp);
        n = numDigits(headListAP1);
        fseek(fp,n*sizeof(char),0);
    }*/
    return fp;
}

FILE *fileOpen(char *fileName, int *head)
{
    FILE *fp;
    char valorCab[5] = "\0";
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
            //*head = atoi(valorCab);
            if (aux != 0)
                *head = atoi(valorCab);
    }
    else{
        itoa(*head,valorCab,10);
        fwrite(valorCab,sizeof(int),1,fp);
        //fclose(fp);
        //fp = fopen(fileName,"r+b");
    }

    return fp;
}
