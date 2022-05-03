#ifndef INC_9_BTREE_BTREE_H
#define INC_9_BTREE_BTREE_H

typedef struct noB noB;
typedef struct arvore arvoreB;

//****FUNÇÕES BÁSICAS******///////////////

arvoreB * criaArvore(int ordem);

noB *alocaNo(arvoreB *A, int f);
//Função que aloca um novo nó
//A ordem do nó é informado em A
//Se o nó é folha ou não é informado em f (1 para folha e 0 para não-folha).

void imprimeArvore(noB *raiz);
//Imprime a raiz e os nós da esquerda para a direita, por nível

noB* retornaRaiz(arvoreB *A);
//Retorna a raiz da árvore

//****FUNÇÕES DE INSERÇÃO******///////////////

void insereElemento(arvoreB *A, int chave);
//Insere um elemento na folha de uma árvore B

void insereArquivo(arvoreB *A, char nomeArquivo[]);
//Lê dados de um arquivo e insere em uma árvore B, chamando a função insereElemento

void split(arvoreB *A, noB *noCheio, int chave);
//Divide um nó cheio em dois - Operação de SPLIT

//****FUNÇÕES DE REMOÇÃO******///////////////

void removeElemento(arvoreB *A, int chave);
//Remove um elemento da árvore B

void rotacaoEsq(noB * noRem, int pos);

void rotacaoDir(noB * noRem, int pos);

void merge(arvoreB *A, noB * noIrmao, noB * noRem, int pos);

#endif //INC_9_BTREE_BTREE_H