/*************************************************************
---------------- Definicao das estruturas---------------------
**************************************************************/
typedef struct registroAP1{
    int codigoControle;
    int codigoCachorro;
    char vacina[50];
    char dataVacina[50];
    char respVacina[50];
} RegAP1; //RegAP1 vai receber as estruturas do Arquivo Principal 1

typedef struct idxAP1{
    int idx_codControle;
    int offset;
    int flag;
} IDX_RegAP1; //IDX_RegAP1 sera a estrutura de indices do AP1


typedef struct registroAP2{
    int codigoCachorro;
    char raca[50];
    char nomeCachorro[50];
} RegAP2; //RegAP2 vai receber as estruturas do Arquivo Principal 2

typedef struct registroDispo{
    int tam;
    int offsetProx;
}RegDispo;
/*******************************************************************/


int pesquisaKeyPrimariaAP2(int ch, FILE *fileAP2);
int insereAP2(RegAP2 *novoAp2);
int obterDadosCadastroAP2();
