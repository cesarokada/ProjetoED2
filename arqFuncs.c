/***************************************************************************
O arquivo arqFuncs.c vai conter as fun��es que envolvem abertura de arquivos
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
    char valorCab[5];
    int size;

    if((fp = fopen(fileName,"r+b")) == NULL){
        fclose(fp);
        if((fp = fopen(fileName,"w+b")) == NULL)
            printf("ERRO AO ABRIR O ARQUIVO\n");
    }
    fseek(fp,0,SEEK_END);
    size = ftell(fp);
    rewind(fp);

    fread(valorCab,sizeof(int),1,fp);
    *head = atoi(valorCab);

    if (size){
        fp = posicionaCabeca(fp,&head);
    }
    else{
        char num[15];
        itoa(*head,num,10);
        fwrite(num,sizeof(int),1,fp);
        printf("\n%d",*head);
        fclose(fp);
        fp = fopen(fileName,"r+b");
    }

    return fp;
}