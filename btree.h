#ifndef INC_9_BTREE_BTREE_H
#define INC_9_BTREE_BTREE_H

typedef struct bNode bNode;
typedef struct tree bTree;

//****FUNÇÕES BÁSICAS******///////////////

bTree * createBTree(int order);

bNode *allocateNode(bTree *T, int f);
//Função que aloca um novo nó
//A ordem do nó é informado em A
//Se o nó é folha ou não é informado em f (1 para folha e 0 para não-folha).

void printBTree(bNode *raiz);
//Imprime a raiz e os nós da esquerda para a direita, por nível

bNode* getRootNode(bTree *T);
//Retorna a raiz da árvore

//****FUNÇÕES DE INSERÇÃO******///////////////

void insertElement(bTree *T, int key);
//Insere um elemento na folha de uma árvore B

void insertFile(bTree *T, char fileName[]);
//Lê dados de um arquivo e insere em uma árvore B, chamando a função insereElemento

void split(bTree *T, bNode *noCheio, int key);
//Divide um nó cheio em dois - Operação de SPLIT

//****FUNÇÕES DE REMOÇÃO******///////////////

void removeElement(bTree *T, int key);
//Remove um elemento da árvore B

void leftRotation(bNode * noRem, int position);

void rightRotation(bNode * noRem, int position);

void merge(bTree *T, bNode * noIrmao, bNode * noRem, int position);

#endif //INC_9_BTREE_BTREE_H