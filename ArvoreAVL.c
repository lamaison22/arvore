#include <stdio.h>
#include <stdlib.h>

typedef struct no{
    int valor;
    struct no *esquerdo,*direito;
    int altura;
}No;


No* novoNo(int x){
    No* novo=malloc(sizeof(No));
    if(novo){ //cria um novo nó, com altura 0 pq sempre é um nó folha e tb sem direita e esquerda
        novo->valor=x;
        novo->esquerdo=NULL;
        novo->direito=NULL;
        novo->altura=0;
    }
    else{
        printf("erro");
    }
    return novo;
}
int maior(int a ,int b){
    if(a>b){
        return a;
    }
    else{
        return b;
    }
}

int alturaNo(No *no){
    if(no==NULL){ //se nao existe retorna -1 para os calculos de balanceamento
        return -1;
    }
    else{  //se nao retorna a propria altura
        return no->altura;
    }
}

int fatorBalanceamento(No *no){     //pode variar de -1 ate 1, se passar disso p cima ou p baixo ta desbalanceado
                                    //se negativo é p direita e se positivo é pra esquerda
    if(no){
        return(alturaNo(no->esquerdo) - alturaNo(no->direito));

    }
    else{
        return 0;
    }
}

//rotacoes
No* rotacaoEsquerda(No *raiz){
    No *y, *f;

    y=raiz->direito;
    f=y->esquerdo; // caso exista salva o filho do no

    y->esquerdo=raiz;
    raiz->direito=f; //como ele tava a direita da raiz qr dizer q era maior entao tem q ser posto a direita da raiz dps q mudar ela de lugar

    raiz->altura=maior(alturaNo(raiz->esquerdo),alturaNo(raiz->direito))+1; // para atualizar as alturas/lugar
    y->altura=maior(alturaNo(y->esquerdo),alturaNo(y->direito))+1;
    
    
    return y;

}
No* rotacaoDireita(No *raiz){
    No *y, *f;

    y=raiz->esquerdo;
    f=y->direito;

    y->direito=raiz;
    raiz->esquerdo=f;

    raiz->altura=maior(alturaNo(raiz->esquerdo),alturaNo(raiz->direito))+1;  
    y->altura=maior(alturaNo(y->esquerdo),alturaNo(y->direito))+1;

    return y;

}
No * rotacaoDireitaEsquerda(No *raiz){ //ela pende pra direita, e depois pra esquerda
    raiz->direito=rotacaoDireita(raiz->direito); //primeiro bota tudo pra direita depois arruma colocando pra esquerda
    return rotacaoEsquerda(raiz);
}
No * rotacaoEsquerdaDireita(No *raiz){ //ela pende pra esquerda, e depois pra direita
    raiz->esquerdo=rotacaoEsquerda(raiz->esquerdo); //primeiro bota tudo pra esquerda depois arruma colocando pra direita
    return rotacaoDireita(raiz);
}


No * balancear(No *raiz){
    int fator=fatorBalanceamento(raiz);
    //rotacao p esquerda
    if(fator<-1 && fatorBalanceamento(raiz->direito)<=0){
        raiz=rotacaoEsquerda(raiz);
    }
    //rotacao p direita
    else if(fator>1&&fatorBalanceamento(raiz->esquerdo)>=0){
        raiz=rotacaoDireita(raiz);
    }
    //rot dupla esquerda
    else if(fator>1&&fatorBalanceamento(raiz->esquerdo)<0){
        raiz=rotacaoEsquerdaDireita(raiz);
    }
    //rot dupla direita
    else if(fator<1 &&fatorBalanceamento(raiz->direito)>0){
        raiz=rotacaoDireitaEsquerda(raiz);
    }

    return raiz;
}

//insercoes e remocoes

No* inserir(No *raiz, int valor){
    if(raiz==NULL){ //arvore vazia
        return novoNo(valor);
    }
    else{ //insere a esquerda ou direita
        if(valor<raiz->valor){
            raiz->esquerdo=inserir(raiz->esquerdo,valor);
        }
        else if(valor >raiz->valor){
            raiz->direito=inserir(raiz->direito,valor);
        }
        else{
            printf("insercao nao realizada \n o elemento %d existe \n",valor);
        }
    }

        //recalcula altura de todos os nos entre a raiz e o novo no
        raiz->altura=maior(alturaNo(raiz->esquerdo),alturaNo(raiz->direito))+1;
        //verfica a necessidade de rebalancear a arvore
        raiz=balancear(raiz);
        return raiz;

}



//remocao
No * remover(No* raiz, int chave){
    if(raiz==NULL){
        printf("Valor nao encontrado\n");
        return NULL;
    }
    else{ //entao procura o valor apontado
         if(raiz->valor ==chave){
             if(raiz->esquerdo==NULL && raiz->direito==NULL){
                 free(raiz);
                 printf("elemento folha removido %d \n",chave);
                 return NULL;
             }
             else{
                 //remove nos q tem duas folhas
                 if(raiz->esquerdo!=NULL &&raiz->direito!=NULL){
                     No* aux=raiz->esquerdo;
                     while(aux->direito!=NULL){
                         aux=aux->direito;
                     }
                     raiz->valor=aux->valor;
                     aux->valor=chave;
                     printf("Elemento trocado %d \n",chave);
                     raiz->esquerdo=remover(raiz->esquerdo,chave);
                     return raiz;
                 }
                  else{
                        No *aux;
                        if(raiz->esquerdo!=NULL){
                            aux=raiz->esquerdo;
                        }
                            else{
                                aux=raiz->direito;
                            }
                            free(raiz);
                            printf("Elemento com 1 filho removido %d \n",chave);
                            return aux;
                 }
             }

             }
             else{
                 if(chave<raiz->valor){
                      raiz->esquerdo=remover(raiz->esquerdo,chave);  
                 }
                 else{
                     raiz->direito=remover(raiz->direito,chave);
                 }
             }
             //recalcula a altura de todos os nos entre a raiz e o novo no 
             raiz->altura=maior(alturaNo(raiz->esquerdo),alturaNo(raiz->direito))+1;

             //verifica a necessidade de rebalancear a arvore
             raiz=balancear(raiz);
             
             return raiz;
            
         }
         
    }
    void imprimir(No * raiz, int nivel){
        int i;
        if(raiz){ //enquanto nao nulo
            imprimir(raiz->direito,nivel+1);
            printf("\n\n");

            for(i=0;i<nivel;i++){
                printf("\t");
            }
            printf("%d",raiz->valor);
            imprimir(raiz->esquerdo,nivel+1);
        }
    }


int main(){
    int opcao, valor;
    No *raiz=NULL;
    do{
        printf("\n\n 0- sair \n \t 1- Inserir \n\t 2-Remover \n\t 3-Imprimir \n\n");
        scanf("%d",&opcao);
        switch(opcao){
            case 0:
            printf("Saindo");
            break;
            
            case 1:
            printf("\t Digite o valor a ser inserido: ");
            scanf("%d",&valor);
            raiz=inserir(raiz,valor);
            break;

            case 2:
            printf("\t Digite o valor a ser removido: ");
            scanf("%d",&valor);
            raiz=remover(raiz,valor);
            break;

            case 3:
            imprimir(raiz,1);
            break;

            default:
            printf("\n Opcao invalida\n");
        }

    }while(opcao!=0);
        
        return 0;

}
