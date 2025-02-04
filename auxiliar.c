#include "auxiliar.h"

//MENU
int menu(){
  int op;
  char strop[10];
  printf("O que deseja fazer? \n");
  printf("[9] Imprimir catalogo estilizado\n");
  printf("[0] Imprimir catalogo\n");
  printf("[1] Inserir filme\n");
  printf("[2] Remover filme\n");
  printf("[3] Buscar filme\n");
  printf("[4] Alterar informacoes de um filme\n");
  printf("[5] Buscar filmes por diretor\n");
  printf("[6] Retirar filmes por diretor\n");
  printf("[7] Retirar filmes por genero\n");
  printf("[8] Retirar filmes por franquia\n");
  printf("[-1] Encerrar programa\n");
  printf("[-2] Encerrar programa sem salvar alteracoes\n");
  
  fgets(strop, 10, stdin);
  strop[strcspn(strop, "\n")] = 0;
  op = atoi(strop);
  return op;
}

void TCat_escreve_Catalogo(TCat *a, FILE * fp){
  if(a){
    int i;
    for(i=0; i <= a->nchaves-1; i++){
      TCat_escreve_Catalogo(a->filho[i], fp);
      fprintf(fp, "%s/%d/%s/%s/%d\n", a->chave[i].titulo,a->chave[i].ano,a->chave[i].diretor,a->chave[i].genero, a->chave[i].duracao);
    }
    TCat_escreve_Catalogo(a->filho[i], fp);
  }
}

//ENCONTRA SUBSTRING
int encontra_Substring(char* string, char*substring){
  char* c = strstr(string, substring);
  if(c) return 1;
  return 0;
}

//RECEBE UMA LINHA DO TIPO: The Avengers/2012/Joss Whedon/Action/153 E RETORNA UM TFILME
TF gera_Filme(char* linha){
    TF filme;
    char* token = strtok(linha, "/");
    strcpy(filme.titulo, token);
    token = strtok(NULL, "/");
    filme.ano = atoi(token);
    token = strtok(NULL, "/");
    strcpy(filme.diretor, token);
    token = strtok(NULL, "/");
    strcpy(filme.genero, token);
    token = strtok(NULL, "/");
    filme.duracao = atoi(token);
    return filme;
}

void printa_Filme(TF filme){
  printf("\n");
  printf("Titulo: %s\nDiretor: %s\nAno de Lancamento: %d\nGenero: %s\nDuracao: %d minutos\n", filme.titulo, filme.diretor, filme.ano, filme.genero, filme.duracao);
  printf("\n");
}

TCat* gera_Arvore(char* arquivo, int t){
    TCat* catalogo;
    FILE *fp = fopen(arquivo, "r");
    if(!fp){
        printf("Erro na abertura do arquivo !\n");
        exit(1);
    }
    int r1;
    char linha[200];
    catalogo = TCat_Inicializa();
    r1 = fscanf(fp, "%[^\n]%*c", linha);
    while(r1 != EOF){
        TF filme = gera_Filme(linha);
        catalogo = TCat_Insere(catalogo, filme, t);
        r1 = fscanf(fp, "%[^\n]%*c", linha);
    }
    fclose(fp);
    return catalogo;
}


//FUNCOES BASE DO TIPO CATALOGO

//CRIACAO
TCat *TCat_Cria(int t){
  TCat *novo = (TCat*)malloc(sizeof(TCat));
  novo->nchaves = 0;
  novo->chave =(TF*)malloc(sizeof(TF)*((t*2)-1));
  novo->folha=1;
  novo->filho = (TCat**)malloc(sizeof(TCat*)*t*2);
  int i;
  for(i = 0; i < (t*2); i++) novo->filho[i] = NULL;
  return novo;
}

//LIBERACAO
TCat *TCat_Libera(TCat *a){
  if(a){
    if(!a->folha){
      int i;
      for(i = 0; i <= a->nchaves; i++) TCat_Libera(a->filho[i]);
    }
    free(a->chave);
    free(a->filho);
    free(a);
    return NULL;
  }
  return a;
}


//IMPRIME
void TCat_Imprime_Catalogo(TCat *a){
  if(a){
    int i;
    for(i=0; i <= a->nchaves-1; i++){
      TCat_Imprime_Catalogo(a->filho[i]);
      printf("%s (%d)\n", a->chave[i].titulo, a->chave[i].ano);
    }
    TCat_Imprime_Catalogo(a->filho[i]);
  }
}

//INICIALIZA
TCat *TCat_Inicializa(){
  return NULL;
}

//BUSCAS

TCat *TCat_Busca_TFilme(TCat* x, TF filme){
  if(!x) return NULL;
  int i = 0;
  while(i < x->nchaves && (strcmp(filme.titulo, x->chave[i].titulo) > 0)) i++;
  if(i < x->nchaves && (strcmp(filme.titulo, x->chave[i].titulo) == 0) && (filme.ano == x->chave[i].ano)) return x;
  if((i < x->nchaves) && (strcmp(filme.titulo, x->chave[i].titulo) == 0) && (filme.ano > x->chave[i].ano)) i++;
  if(x->folha) return NULL;
  return TCat_Busca_TFilme(x->filho[i], filme);
}

TCat *TCat_Busca_Filme(TCat* x, char* titulo, int ano){
    if(!x) return NULL;
    int i = 0;
    while((i < x->nchaves) && ((strcmp(titulo, x->chave[i].titulo) > 0))) i++;
    if((i < x->nchaves) && (strcmp(titulo, x->chave[i].titulo) == 0) && (ano == x->chave[i].ano)) return x;
    if((i < x->nchaves) && (strcmp(titulo, x->chave[i].titulo) == 0) && (ano > x->chave[i].ano)) i++;
    if(x->folha) return NULL;
    return TCat_Busca_Filme(x->filho[i], titulo, ano);
}

TCat *TCat_Busca_Titulo(TCat* x, char* titulo){
  if(!x) return NULL;
  int i = 0;
  while(i < x->nchaves && (strcmp(titulo, x->chave[i].titulo) > 0)) i++;
  if(i < x->nchaves && (strcmp(titulo, x->chave[i].titulo) == 0)) return x;
  if(x->folha) return NULL;
  return TCat_Busca_Titulo(x->filho[i], titulo);
}


TF *TCat_Aponta_Filme(TCat* x, char* titulo, int ano){
  if(!x) return NULL;
    int i = 0;
    while((i < x->nchaves) && ((strcmp(titulo, x->chave[i].titulo) > 0))) i++;
    if((i < x->nchaves) && (strcmp(titulo, x->chave[i].titulo) == 0) && (ano == x->chave[i].ano)) {
      TF *p;
      p = &x->chave[i];
      return p;
    }
    if((i < x->nchaves) && (strcmp(titulo, x->chave[i].titulo) == 0) && (ano > x->chave[i].ano)) i++;
    if(x->folha) {
      printf("Filme nao encontrado!\n");
      return NULL;
    }
    return TCat_Aponta_Filme(x->filho[i], titulo, ano);
}




//INSERE E AUXILIARES

TCat *Divisao(TCat *x, int i, TCat* y, int t){
  TCat *z= TCat_Cria(t);
  z->nchaves= t - 1;
  z->folha = y->folha;
  int j;
  for(j=0;j<t-1;j++) z->chave[j] = y->chave[j+t];
  if(!y->folha){
    for(j=0;j<t;j++){
      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = NULL;
    }
  }
  y->nchaves = t-1;
  for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
  x->filho[i] = z;
  for(j=x->nchaves; j>=i; j--) x->chave[j] = x->chave[j-1];
  x->chave[i-1] = y->chave[t-1];
  x->nchaves++;
  return x;
}

TCat *Insere_Nao_Completo(TCat *x, TF filme, int t){
  int i = x->nchaves-1;
  if(x->folha){
    while((i>=0) && ((strcmp(filme.titulo, x->chave[i].titulo) < 0) || ((strcmp(filme.titulo, x->chave[i].titulo) == 0) && (filme.ano > x->chave[i].ano)))){
      x->chave[i+1] = x->chave[i];
      i--;
    }
    x->chave[i+1] = filme;
    x->nchaves++;
    return x;
  }
  while((i>=0) && ((strcmp(filme.titulo, x->chave[i].titulo) < 0) || ((strcmp(filme.titulo, x->chave[i].titulo) == 0) && (filme.ano > x->chave[i].ano)))) i--;
  i++;
  if(x->filho[i]->nchaves == ((2*t)-1)){
    x = Divisao(x, (i+1), x->filho[i], t);
    if(((strcmp(filme.titulo, x->chave[i].titulo) > 0) || ((strcmp(filme.titulo, x->chave[i].titulo) == 0) && (filme.ano < x->chave[i].ano)))) i++;
  }
  x->filho[i] = Insere_Nao_Completo(x->filho[i], filme, t);
  return x;
}

TCat *TCat_Insere(TCat *T, TF filme, int t){
  /*if(TCat_Busca_TFilme(T,filme)) {
    printf("O filme ja esta no catalogo!\n");
    return T;
  }*/
  if(!T){
    T = TCat_Cria(t);
    T->chave[0] = filme;
    T->nchaves=1;
    return T;
  }
  if(T->nchaves == (2*t)-1){
    TCat *s = TCat_Cria(t);;
    s->nchaves=0;
    s->folha = 0;
    s->filho[0] = T;
    s = Divisao(s,1,T,t);
    s = Insere_Nao_Completo(s,filme,t);
    return s;
  }
  T = Insere_Nao_Completo(T,filme,t);
  return T;
}

TCat *TCat_Libera_Remocao(TCat *a){
  free(a->chave);
  free(a->filho);
  free(a);
  return NULL;
}


//RETIRADA
//[5] Buscar filmes por diretor
//[6] Retirar filmes por diretor
//[7] Retirar filmes por genero


TCat* removerFilme(TCat* arv, char* titulo, int ano, int t){
  if(!arv) return arv;
  int i;
  //printf("Removendo %d ...\n", ch);
  //for(i = 0; i<arv->nchaves && arv->chave[i] < ch; i++);
  for(i = 0; i<arv->nchaves && ((strcmp(titulo, arv->chave[i].titulo) > 0) || ((strcmp(titulo, arv->chave[i].titulo) == 0) && (ano < arv->chave[i].ano))); i++);
  //if(i < arv->nchaves && ch == arv->chave[i]){ //CASOS 1, 2A, 2B e 2C
  if(i < arv->nchaves && ((strcmp(titulo, arv->chave[i].titulo) == 0) && (ano == arv->chave[i].ano))){
    if(arv->folha){ //CASO 1
      //printf("\nCASO 1\n");
      int j;
      for(j=i; j<arv->nchaves-1;j++) arv->chave[j] = arv->chave[j+1];
      arv->nchaves--;
      if(!arv->nchaves){
        //TARVB_Libera(arv);
        TCat_Libera(arv);
        arv = NULL;
      }
      return arv;      
    }
    if(!arv->folha && arv->filho[i]->nchaves >= t){ //CASO 2A
      //printf("\nCASO 2A\n");
      //TARVB *y = arv->filho[i];
      TCat *y = arv->filho[i];  //Encontrar o predecessor k' de k na árvore com raiz em y
      while(!y->folha) y = y->filho[y->nchaves];
      //int temp = y->chave[y->nchaves-1];
      TF temp = y->chave[y->nchaves-1];
      //arv->filho[i] = remover(arv->filho[i], temp, t); 
      arv->filho[i] = removerFilme(arv->filho[i], temp.titulo, temp.ano, t); 
      //Eliminar recursivamente K e substitua K por K' em x
      arv->chave[i] = temp;
      return arv;
    }
    if(!arv->folha && arv->filho[i+1]->nchaves >= t){ //CASO 2B
      //printf("\nCASO 2B\n");
      //TARVB *y = arv->filho[i+1]; //Encontrar o sucessor k' de k na árvore com raiz em y
      TCat *y = arv->filho[i+1];
      while(!y->folha) y = y->filho[0];
      //int temp = y->chave[0];
      TF temp = y->chave[0];
      //y = remover(arv->filho[i+1], temp, t); //Eliminar recursivamente K e substitua K por K' em x
      y = removerFilme(arv->filho[i+1], temp.titulo, temp.ano, t); 
      arv->chave[i] = temp;
      return arv;
    }
    if(!arv->folha && arv->filho[i+1]->nchaves == t-1 && arv->filho[i]->nchaves == t-1){ //CASO 2C
      //printf("\nCASO 2C\n");
      //TARVB *y = arv->filho[i];
      //TARVB *z = arv->filho[i+1];
      TCat *y = arv->filho[i];
      TCat *z = arv->filho[i+1];
      //y->chave[y->nchaves] = ch;         //colocar ch ao final de filho[i]
      TCat *x = TCat_Busca_Filme(arv, titulo, ano);
      int c = 0;
      while(1){
        if((strcmp(titulo, x->chave[c].titulo) == 0) && (ano == x->chave[c].ano)) {
          y->chave[y->nchaves] = x->chave[c];
          break;
        }
        c++;  
      }
      int j;
      for(j=0; j<t-1; j++)                //juntar chave[i+1] com chave[i]
        y->chave[t+j] = z->chave[j];
      for(j=0; j<t; j++){                 //juntar filho[i+1] com filho[i]
        y->filho[t+j] = z->filho[j];
        z->filho[j] = NULL;
      }
      y->nchaves = 2*t-1;
      for(j=i; j < arv->nchaves-1; j++)   //remover ch de arv
        arv->chave[j] = arv->chave[j+1];
      for(j=i+1; j < arv->nchaves; j++)  
        arv->filho[j] = arv->filho[j+1]; //remover ponteiro para filho[i+1]
      arv->filho[j] = NULL;
      //Libera_Remocao(z);
      TCat_Libera_Remocao(z);
      arv->nchaves--;
      if(!arv->nchaves){
        //TARVB *temp = arv;
        TCat *temp = arv;
        arv = arv->filho[0];
        temp->filho[0] = NULL;
        //Libera_Remocao(temp);
        TCat_Libera_Remocao(temp);
        //arv = remover(arv, ch, t);
        arv = removerFilme(arv, titulo, ano, t);
      }
      //else arv->filho[i] = remover(arv->filho[i], ch, t);
      else arv->filho[i] = removerFilme(arv->filho[i], titulo, ano, t);
      return arv;
    }   
  }

  //TARVB *y = arv->filho[i], *z = NULL;
  TCat* y = arv->filho[i], *z = NULL;
  if (y->nchaves == t-1){ //CASOS 3A e 3B
    if((i < arv->nchaves) && (arv->filho[i+1]->nchaves >=t)){ //CASO 3A
      z = arv->filho[i+1];
      y->chave[t-1] = arv->chave[i];   //dar a y a chave i da arv
      y->nchaves++;
      arv->chave[i] = z->chave[0];     //dar a arv uma chave de z
      int j;
      for(j=0; j < z->nchaves-1; j++)  //ajustar chaves de z
        z->chave[j] = z->chave[j+1];
      y->filho[y->nchaves] = z->filho[0]; //enviar ponteiro menor de z para o novo elemento em y
      for(j=0; j < z->nchaves; j++)       //ajustar filhos de z
        z->filho[j] = z->filho[j+1];
      z->nchaves--;
      //arv->filho[i] = remover(arv->filho[i], ch, t);
      arv->filho[i] = removerFilme(arv->filho[i], titulo, ano, t);
      return arv;
    }
    if((i > 0) && (!z) && (arv->filho[i-1]->nchaves >=t)){ //CASO 3A
      z = arv->filho[i-1];
      int j;
      for(j = y->nchaves; j>0; j--)               //encaixar lugar da nova chave
        y->chave[j] = y->chave[j-1];
      for(j = y->nchaves+1; j>0; j--)             //encaixar lugar dos filhos da nova chave
        y->filho[j] = y->filho[j-1];
      y->chave[0] = arv->chave[i-1];              //dar a y a chave i da arv
      y->nchaves++;
      arv->chave[i-1] = z->chave[z->nchaves-1];   //dar a arv uma chave de z
      y->filho[0] = z->filho[z->nchaves];         //enviar ponteiro de z para o novo elemento em y
      z->nchaves--;
      //arv->filho[i] = remover(y, ch, t);]
      arv->filho[i] = removerFilme(y, titulo, ano, t);
      return arv;
    }
    if(!z){ //CASO 3B
      if(i < arv->nchaves && arv->filho[i+1]->nchaves == t-1){
        z = arv->filho[i+1];
        y->chave[t-1] = arv->chave[i];     //pegar chave [i] e coloca ao final de filho[i]
        y->nchaves++;
        int j;
        for(j=0; j < t-1; j++){
          y->chave[t+j] = z->chave[j];     //passar filho[i+1] para filho[i]
          y->nchaves++;
        }
        if(!y->folha){
          for(j=0; j<t; j++){
            y->filho[t+j] = z->filho[j];
            z->filho[j] = NULL;
          }
        }
        TCat_Libera(z);
        for(j=i; j < arv->nchaves-1; j++){ //limpar referências de i
          arv->chave[j] = arv->chave[j+1];
          arv->filho[j+1] = arv->filho[j+2];
        }
        arv->filho[arv->nchaves] = NULL;
        arv->nchaves--;
        if(!arv->nchaves){
          //TARVB *temp = arv;
          TCat *temp = arv;
          arv = arv->filho[0];
          temp->filho[0] = NULL;
          //TARVB_Libera(temp);
          TCat_Libera(temp);
        }
        //arv = remover(arv, ch, t);
        arv = removerFilme(arv, titulo, ano, t);
        return arv;
      }
      if((i > 0) && (arv->filho[i-1]->nchaves == t-1)){ 
        z = arv->filho[i-1];
        if(i == arv->nchaves)
          z->chave[t-1] = arv->chave[i-1]; //pegar chave[i] e poe ao final de filho[i-1]
        else
          z->chave[t-1] = arv->chave[i];   //pegar chave [i] e poe ao final de filho[i-1]
        z->nchaves++;
        int j;
        for(j=0; j < t-1; j++){
          z->chave[t+j] = y->chave[j];     //passar filho[i+1] para filho[i]
          z->nchaves++;
        }
        if(!z->folha){
          for(j=0; j<t; j++){
            z->filho[t+j] = y->filho[j];
            y->filho[j] = NULL;
          }
        }
        //TARVB_Libera(y);
        TCat_Libera(y);
        arv->filho[arv->nchaves] = NULL;
        arv->nchaves--;
        if(!arv->nchaves){
          //TARVB *temp = arv;
          TCat *temp = arv;
          arv = arv->filho[0];
          temp->filho[0] = NULL;
          //TARVB_Libera(temp);
          TCat_Libera(temp);
        }
        else arv->filho[i-1] = z;
        //arv = remover(arv, ch, t);
        arv = removerFilme(arv, titulo, ano, t);
        return arv;
      }
    }
  }  
  //arv->filho[i] = remover(arv->filho[i], ch, t);
  arv->filho[i] = removerFilme(arv->filho[i], titulo, ano, t);
  return arv;
}


TCat* TCat_Retira(TCat* arv, char* titulo, int ano, int t){
  /*if(!arv || !TCat_Busca_Filme(arv, titulo, ano)) return arv;*/
  if(!arv) {
    printf("\nO catalogo não existe!\n");
    return arv;
  }
  return removerFilme(arv, titulo, ano, t);
}


TF* TCat_busca_Franquia(TCat *a, char* substring){
  if(!a) return NULL;
  int i;
  TF*p;
  for(i= 0; i<= a->nchaves-1; i++){
    p = TCat_busca_Franquia(a->filho[i], substring);
    if(p) return p;
    if(encontra_Substring(a->chave[i].titulo,substring)){
      p = &a->chave[i];
      printf("%s (%d) removido do catalogo!\n", p->titulo, p->ano);
      return p;
    }
  }
  return TCat_busca_Franquia(a->filho[i], substring);
}


TF* TCat_busca_Genero(TCat *a, char* genero){
  if(!a) return NULL;
  int i;
  TF*p;
  for(i= 0; i<= a->nchaves-1; i++){
    p = TCat_busca_Genero(a->filho[i], genero);
    if(p) return p;
    if((encontra_Substring(a->chave[i].genero, genero)) == 1){
      p = &a->chave[i];
      printf("%s (%d) removido do catalogo!\n", p->titulo, p->ano);
      return p;
    }
  }
  return TCat_busca_Genero(a->filho[i], genero);
}

void TCat_busca_Diretor(TCat*a, char* diretor){
  if(!a) return;
  int i;
  for(i= 0; i<= a->nchaves-1; i++){
    TCat_busca_Diretor(a->filho[i], diretor);
    if((strcmp(a->chave[i].diretor, diretor)) == 0){
      printa_Filme(a->chave[i]);
    }
  }
  TCat_busca_Diretor(a->filho[i], diretor);
}

TF* TCat_remove_Diretor(TCat *a, char* diretor){
  if(!a) return NULL;
  int i;
  TF*p;
  for(i= 0; i<= a->nchaves-1; i++){
    p = TCat_remove_Diretor(a->filho[i], diretor);
    if(p) return p;
    if((strcmp(a->chave[i].diretor, diretor)) == 0){
      p = &a->chave[i];
      printf("%s (%d) removido do catalogo!\n", p->titulo, p->ano);
      return p;
    }
  }
  return TCat_remove_Diretor(a->filho[i], diretor);
}




//FUNCOES DE USUARIO

//BUSCA FILME
void usuario_Busca(TCat *catalogo){
  char titulo[81], strano[10];
  int ano;
  printf("\nBuscando filme... \n");
  printf("Digite o titulo do filme: ");
  fgets(titulo, 81, stdin);
  titulo[strcspn(titulo, "\n")] = 0;
  printf("Digite o ano de lancamento do filme: ");
  fgets(strano, 10, stdin);
  strano[strcspn(strano, "\n")] = 0;
  ano = atoi(strano);
  TF *p = TCat_Aponta_Filme(catalogo, titulo, ano);
  if(!p) {
    printf("\n Filme nao encontrado!\n");
    return;
  }
  printf("\nFilme encontrado:\n");
  printa_Filme(*p);
}

//INSERE FILME
TCat* usuario_Insere(TCat *catalogo, int t){
  TF filme;
  char strano[10], strduracao[10];
  printf("\nInserindo filme...\n");
  printf("Digite o titulo do filme: ");
  fgets(filme.titulo, 81, stdin);
  filme.titulo[strcspn(filme.titulo, "\n")] = 0;
  printf("Digite o nome do diretor do filme: ");
  fgets(filme.diretor, 51, stdin);
  filme.diretor[strcspn(filme.diretor, "\n")] = 0;
  printf("Digite o genero do filme: ");
  fgets(filme.genero, 31, stdin);
  filme.genero[strcspn(filme.genero, "\n")] = 0;
  printf("Digite o ano de lancamento do filme: ");
  fgets(strano, 10, stdin);
  strano[strcspn(strano, "\n")] = 0;
  filme.ano = atoi(strano);
  printf("Digite a duracao do filme em minutos: ");
  fgets(strduracao, 10, stdin);
  strduracao[strcspn(strduracao, "\n")] = 0;
  filme.duracao = atoi(strduracao);
  if(filme.ano == 0){
      printf("\nAno inserido invalido!\n");
      return catalogo;
    }
  if(filme.duracao == 0){
      printf("\nDuracao inserida invalida!\n");
      return catalogo;
    }
  if(TCat_Busca_TFilme(catalogo,filme)){
    printf("\nO filme ja esta no catalogo!\n");
    return catalogo;
  }
  printf("\nFilme inserido com sucesso!\n");
  printa_Filme(filme);
  catalogo = TCat_Insere(catalogo, filme, t);
  return catalogo;
}

//REMOVE FILME
TCat* usuario_Remove(TCat* catalogo, int t){
  printf("\nRemovendo filme do catalogo... \n");
  char titulo[81], strano[10];
  int ano;
  printf("Digite o titulo do filme: ");
  fgets(titulo, 81, stdin);
  titulo[strcspn(titulo, "\n")] = 0;
  printf("Digite o ano de lancamento do filme: ");
  fgets(strano, 10, stdin);
  strano[strcspn(strano, "\n")] = 0;
  ano = atoi(strano);
  if(ano == 0){
      printf("\nAno inserido invalido!\n");
      return catalogo;
  }
  printf("Removendo %s (%d) do catalogo... \n", titulo, ano);
  if(!TCat_Busca_Filme(catalogo, titulo, ano)){
    printf("\nO filme nao esta no catalogo!\n");
    return catalogo;
  }
  catalogo = TCat_Retira(catalogo, titulo, ano, t);
  printf("\n%s (%d) removido com sucesso!\n", titulo, ano);
  return catalogo;
}

//ALTERA CHAVES SECUNDARIAS
TCat* usuario_Altera(TCat *catalogo){
  int ano, duracao;
  char strduracao[10], diretor[51], genero[31], titulo[81], strano[10];
  printf("\nAlterando informacoes...\n");
  printf("Digite o titulo do filme: \n");
  fgets(titulo, 81, stdin);
  titulo[strcspn(titulo, "\n")] = 0;
  printf("Digite o ano de lancamento: \n");
  fgets(strano, 10, stdin);
  strano[strcspn(strano, "\n")] = 0;
  ano = atoi(strano);
  if(ano == 0){
      printf("\nAno inserido invalido!\n");
      return catalogo;
    }
  TF* p = TCat_Aponta_Filme(catalogo, titulo, ano);
  if(!p) {
    printf("\nFilme nao encontrado!\n");
    return catalogo;
  }
  printf("\nFilme selecionado: \n");
  printa_Filme(*p);
  printf("Alterando informacoes de %s (%d)...\n", titulo, ano);
  printf("Digite o nome do diretor: \n");
  fgets(diretor,  51, stdin);
  diretor[strcspn(diretor, "\n")] = 0;
  printf("Digite o genero do filme: \n");
  fgets(genero, 31, stdin);
  genero[strcspn(genero, "\n")] = 0;
  printf("Digite a duracao do filme: \n");
  fgets(strduracao, 10, stdin);
  strduracao[strcspn(strduracao, "\n")] = 0;
  duracao = atoi(strduracao);
  if(duracao == 0){
      printf("Duracao invalida!");
      return catalogo;
    }
  strcpy(p->diretor, diretor);
  strcpy(p->genero, genero);
  p->duracao = duracao;
  return catalogo;
}

//USUARIO REMOVE FRANQUIA
TCat* usuario_Remove_Franquia(TCat* catalogo, int t){
  char franquia[81], titulo[81];
  int ano;
  printf("\nRemovendo Franquia...\n");
  printf("Digite a franquia a ser removida: \n");
  fgets(franquia, 81, stdin);
  franquia[strcspn(franquia, "\n")] = 0;
  TF *p = NULL;
  printf("\nFranquia: %s sendo retirada...\n", franquia);
  while(1){
    p = TCat_busca_Franquia(catalogo, franquia);
    if(!p) {
      break;
    }
    strcpy(titulo, p->titulo);
    ano = p->ano;
    catalogo = TCat_Retira(catalogo, titulo, ano, t);
  }
  return catalogo;
}

TCat* usuario_Remove_Genero(TCat* catalogo, int t){
  char genero[51], titulo[81];
  int ano;
  printf("\nRemovendo Filmes do Genero...\n");
  printf("Digite qual genero sera removido do catalogo: \n");
  fgets(genero, 81, stdin);
  genero[strcspn(genero, "\n")] = 0;
  TF *p = NULL;
  printf("\nFilmes do Genero: %s sendo removidos...\n", genero);
  while(1){
    p = TCat_busca_Genero(catalogo, genero);
    if(!p) {
      break;
    }
    strcpy(titulo, p->titulo);
    ano = p->ano;
    catalogo = TCat_Retira(catalogo, titulo, ano, t);
  }
  return catalogo;
}

TCat* usuario_Remove_Diretor(TCat* catalogo, int t){
  char diretor[51], titulo[81];
  int ano;
  printf("\nRemovendo Filmes do Diretor...\n");
  printf("Digite o nome do diretor a ter seus filmes removidos do catalogo: \n");
  fgets(diretor, 81, stdin);
  diretor[strcspn(diretor, "\n")] = 0;
  TF *p = NULL;
  printf("\nDiretor: %s tendo seus filmes removidos...\n", diretor);
  while(1){
    p = TCat_remove_Diretor(catalogo, diretor);
    if(!p) {
      break;
    }
    strcpy(titulo, p->titulo);
    ano = p->ano;
    catalogo = TCat_Retira(catalogo, titulo, ano, t);
  }
  return catalogo;
}

void usuario_Busca_Diretor(TCat* catalogo){
  char diretor[51];
  printf("\nBuscando Filmes do Diretor...\n");
  printf("Digite o nome do diretor a ter seus filmes buscados no catalogo: \n");
  fgets(diretor, 81, stdin);
  diretor[strcspn(diretor, "\n")] = 0;
  printf("\n%s dirige os seguintes filmes do catalogo:\n", diretor);
  TCat_busca_Diretor(catalogo, diretor);
}


//REQUISICAO ROSSETI

void TCat_Imprime_Catalogo_Estilizado_Rec(TCat *a, int andar){
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      TCat_Imprime_Catalogo_Estilizado_Rec(a->filho[i],andar+1);
      for(j=0; j<=andar; j++) printf("\t");
      printf("%s (%d)\n", a->chave[i].titulo, a->chave[i].ano);
    }
    TCat_Imprime_Catalogo_Estilizado_Rec(a->filho[i],andar+1);
  }
}

void TCat_Imprime_Catalogo_Estilizado(TCat *a){
  TCat_Imprime_Catalogo_Estilizado_Rec(a, 0);
}








