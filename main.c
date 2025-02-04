#include "auxiliar.c"

int main(void){
    //DECLARACAO DAS VARIAVEIS
    char strt[10];
    int op, t;
    FILE* fp;
    TCat *catalogo = NULL;

    //ATRIBUICAO
    printf("Digite o grau do Catalogo: ");
    fgets(strt, 10, stdin);
    strt[strcspn(strt, "\n")] = 0;
    t = atoi(strt);
    if(t<2) {
        printf("\nGrau inserido menor que 2, o Catalogo tera grau 2!\n");
        t = 2;
    }
    printf("\n");
    catalogo = gera_Arvore("..\\catalogo.txt", t);        // Lucas ("catalogo.txt", t)        // Fred ("..\\catalogo.txt", t)



    //MENU CATALOGO
    while(1){
        op = menu();
        printf("\n");
        if(op == 9){
            printf("\nCATALOGO ESTILIZADO:\n");
            TCat_Imprime_Catalogo_Estilizado(catalogo);
        }
        else if(op == 0) {
            printf("\nCATALOGO:\n");
            TCat_Imprime_Catalogo(catalogo);
            
        }
        else if(op == -1) {
            fp = fopen("catalogo.txt", "w");
            if(!fp) printf("\nErro ao abrir arquivo!\n");
            else{
                TCat_escreve_Catalogo(catalogo, fp);
                fclose(fp);
                TCat_Libera(catalogo);
                printf("\nPrograma encerrado!\n");
                break;
            }
        }
        else if(op == -2){
            TCat_Libera(catalogo);
            printf("\nPrograma encerrado sem salvar alteracoes!\n");
            break;
        }
        else if(op == 1) catalogo = usuario_Insere(catalogo, t);
        else if(op == 2) catalogo = usuario_Remove(catalogo, t);
        else if(op == 3) usuario_Busca(catalogo);
        else if(op == 4) catalogo = usuario_Altera(catalogo);
        else if(op == 5) usuario_Busca_Diretor(catalogo);
        else if(op == 6) catalogo = usuario_Remove_Diretor(catalogo, t);
        else if(op == 7) catalogo = usuario_Remove_Genero(catalogo, t);
        else if(op == 8) catalogo = usuario_Remove_Franquia(catalogo, t);
        else printf("Operacao invalida, digite novamente!\n");
        printf("\n");
    }

    return 0;
}