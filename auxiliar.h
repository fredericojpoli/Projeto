#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef struct TipoFilme{
  char titulo[81], diretor[51], genero[31];
  int ano, duracao;
}TF;


typedef struct Catalogo{
  int nchaves, folha;
  TF *chave;
  struct Catalogo **filho;
}TCat;

//FUNCOES AUXILIARES
int menu();
void TCat_escreve_Catalogo(TCat *a, FILE * fp);
int encontra_Substring(char* string, char*substring);
TF gera_Filme(char* linha);
void printa_Filme(TF filme);
TCat* gera_Arvore(char* arquivo, int t);



//FUNCOES TCATALOGO
TCat *TCat_Cria(int t);
TCat *TCat_Libera(TCat *a);
void TCat_Imprime_Catalogo(TCat *a);
TCat *TCat_Inicializa();
TCat *TCat_Busca_TFilme(TCat* x, TF filme);
TCat *TCat_Busca_Filme(TCat* x, char* titulo, int ano);
TCat *TCat_Busca_Titulo(TCat* x, char* titulo);
TF* TCat_Aponta_Filme(TCat* catalogo, char* titulo, int ano);
TCat *Divisao(TCat *x, int i, TCat* y, int t);
TCat *Insere_Nao_Completo(TCat *x, TF filme, int t);
TCat *TCat_Insere(TCat *T, TF filme, int t);
TCat *TCat_Libera_Remocao(TCat *a);
TCat* removerFilme(TCat* arv, char* titulo, int ano, int t);
TCat* TCat_Retira(TCat* arv, char* titulo, int ano, int t);
TF* TCat_busca_Franquia(TCat *a, char* substring);
TF* TCat_busca_Genero(TCat *a, char* genero);
void TCat_busca_Diretor(TCat*a, char* diretor);
TF* TCat_remove_Diretor(TCat *a, char* diretor);

//FUNCOES DE USUARIO
void usuario_Busca(TCat *catalogo);
TCat* usuario_Insere(TCat *catalogo, int t);
TCat* usuario_Remove(TCat* catalogo, int t);
TCat* usuario_Altera(TCat *catalogo);
TCat* usuario_Remove_Franquia(TCat* catalogo, int t);
TCat* usuario_Remove_Genero(TCat* catalogo, int t);
TCat* usuario_Remove_Diretor(TCat* catalogo, int t);
void usuario_Busca_Diretor(TCat* catalogo);
