#include <stdio.h>
#include <stdlib.h>

#include "btree.h"

struct bNode {
    int *keys; //array de keys
    struct bNode **pointers; //array de pointers com o endereço para os filhos
    struct bNode *father;
    int occupation; //qunatas keys válidas possui o nó
    int leaf; // 0 não é leaf; 1 é leaf
};

struct tree {
    struct bNode *sentinel;
    int order; // representa o número de filhos que o nó pode ter
};

bTree * createBTree(int order) {
    bTree *T = (bTree*)malloc(sizeof(bTree));//aloca memória para árvore

    if (!T) {
        printf("Erro ao alocar memoria");
        return NULL;
    }

    bNode *sent = NULL; //cria sentinel
    T->order = order; //seta a order da árvore
    T->sentinel = sent; //seta a sentinel na árvore
    return T;
}

bNode *allocateNode(bTree *T, int f) {
    bNode *newNode = (bNode*)malloc(sizeof(bNode)); //aloca memória do novo nó

    if (!newNode) {
        printf("Erro ao alocar memoria");
        return NULL;
    }

    newNode->keys = (int*) calloc(T->order, sizeof(int)); //aloca memória das keys do nó

    if (!newNode->keys) {
        printf("Erro ao alocar memoria");
        return NULL;
    }

    newNode->pointers = (bNode**)calloc(T->order + 1, sizeof(bNode*)); //aloca memória dos pointers para os filhos
    newNode->occupation = 0;
    newNode->leaf = f;
    newNode->father = NULL;

    return newNode;
}

void insertElement(bTree *T, int key) {
    bNode *aux = T->sentinel; //começa como a sentinel
    int i, validation = 0;

    if(aux == NULL) {
        aux = allocateNode(T, 1); //aloca o novo nó na raiz
        aux->keys[0] = key;
        aux->occupation = 1;
        T->sentinel = aux;
        aux->father = T->sentinel;

        return;
    }

    while(aux->leaf == 0) {  //Procurar a leaf correta
        if(aux->occupation == T->order - 1 && aux->leaf == 0 && validation != 1 && T->order % 2 == 0) {
            split(T, aux, key);
            validation = 1;
            aux = aux->father;
        }

        i = 0;
        while ((aux->keys[i] < key) && (i < aux->occupation) && (i < T->order)) {
            i++; //procura o ponteiro correto
        }

        aux = aux->pointers[i];
    }

    i = aux->occupation; //recebe o número de ocupações
    while ((key < aux->keys[i - 1]) && (i > 0)) { //procura o lugar ideal para inserir a key
        aux->keys[i] = aux->keys[i - 1]; //faz o shift nos números
        i--;
    }

    aux->keys[i] = key;
    aux->occupation++; //insere e atualiza a ocupação
    
    if(aux->occupation == T->order) {   //nó leaf está cheio
        split(T, aux, key); //realiza o split passando a árvore, no em que o elemnto seria inserido e a key que será inserida 
    }

    return;
}

void split(bTree *T, bNode *fullNode, int key) {
    if(T->order % 2 != 0) {
        int occupationIntermed = T->order; //serve para realizar a atualização dos valores do irmão
        int middle = T->order/2; //índice middle do vetor de keys
        int middleElement = fullNode->keys[middle]; //elemento key do middle do vetor de keys
        int i, j = 0;

        bNode *brother = allocateNode(T, fullNode->leaf); //aloca o irmão, que é a key do middle nó que estava cheio
        bNode *newRoot;

        if (fullNode == T->sentinel) {  //split de raiz
            newRoot = allocateNode(T, 0); //aloca a nova raiz
            //subir o elemento do middle
            newRoot->occupation = 1; 
            newRoot->keys[0] = middleElement; //coloca o valor do middle do nó que estava cheio no índice 0 da nova raiz
            T->sentinel = newRoot;
            newRoot->father = T->sentinel;
            newRoot->pointers[0] = fullNode; //aloca os filhos da nova raiz
            newRoot->pointers[1] = brother;

            fullNode->occupation--; //atualiza a ocupação
            fullNode->father = newRoot;
        }

        //SPLIT DE NÓ INTERMEDIÁRIO OU leaf
        else {
            i = fullNode->father->occupation; //insere o elemento na posição correta do father
            while(i > 0 && middleElement < fullNode->father->keys[i - 1]) {
                fullNode->father->keys[i] = fullNode->father->keys[i - 1];
                fullNode->father->pointers[i + 1] = fullNode->father->pointers[i];
                i--;
            }

            fullNode->father->keys[i] = middleElement;
            fullNode->father->pointers[i + 1] = brother;
            fullNode->father->occupation++;
            fullNode->occupation--;
        }
        brother->father = fullNode->father;
        //copiar os elementos maior que o middle para o irmão
        //atualizar os pointers
        for(i=middle + 1; i< occupationIntermed; i++, j++) {
            brother->keys[j] = fullNode->keys[i];
            brother->pointers[j] = fullNode->pointers[i];

            if(brother->pointers[j] != NULL){
                brother->pointers[j]->father = brother;
            }
            brother->occupation++;
            fullNode->occupation--;
        }

        brother->pointers[j] = fullNode->pointers[i];
        if(brother->pointers[j] != NULL) {
            brother->pointers[j]->father = brother;
        }

        if(fullNode->father->occupation == T->order) {
            split(T, fullNode->father, key);
        }
    } else {
        int occupationIntermed = T->order; //serve para realizar a atualização dos valores do irmão
        int middle = 0; //índice middle do vetor de keys

        if(fullNode->occupation == T->order - 1) {
            middle = (fullNode->occupation - 1) / 2;
            occupationIntermed = fullNode->occupation;
        } else if(T->order % 2 == 0) {
            middle = (T->order/2) - 1;
        }

        int middleElement = fullNode->keys[middle]; //elemento key do middle do vetor de keys
        int i, j = 0;

        bNode *brother = allocateNode(T, fullNode->leaf); //aloca o irmão, que é a key do middle nó que estava cheio
        bNode *newRoot;

        if (fullNode == T->sentinel) { //split de raiz
            newRoot = allocateNode(T, 0); //aloca a nova raiz
            //subir o elemento do middle
            newRoot->occupation = 1; 
            newRoot->keys[0] = middleElement; //coloca o valor do middle do nó que estava cheio no índice 0 da nova raiz
            T->sentinel = newRoot;
            newRoot->father = T->sentinel;
            newRoot->pointers[0] = fullNode; //aloca os filhos da nova raiz
            newRoot->pointers[1] = brother;

            fullNode->occupation--; //atualiza a ocupação
            fullNode->father = newRoot;
        }

        //SPLIT DE NÓ INTERMEDIÁRIO OU leaf
        else {
            i = fullNode->father->occupation; //insere o elemento na posição correta do father
            while(i > 0 && middleElement < fullNode->father->keys[i - 1]) {
                fullNode->father->keys[i] = fullNode->father->keys[i - 1];
                fullNode->father->pointers[i + 1] = fullNode->father->pointers[i];
                i--;
            }
            fullNode->father->keys[i] = middleElement;
            fullNode->father->pointers[i + 1] = brother;
            fullNode->father->occupation++;
            fullNode->occupation--;
        }
        brother->father = fullNode->father;
        //copiar os elementos maior que o middle para o irmão
        //atualizar os pointers
        for(i=middle + 1; i< occupationIntermed; i++, j++) {
            brother->keys[j] = fullNode->keys[i];
            brother->pointers[j] = fullNode->pointers[i];
            if(brother->pointers[j] != NULL)
                brother->pointers[j]->father = brother;
            brother->occupation++;
            fullNode->occupation--;
        }

        brother->pointers[j] = fullNode->pointers[i];
        if(brother->pointers[j] != NULL) {
            brother->pointers[j]->father = brother;
        }
    }
}

void leftRotation(bNode * noRem, int pos) {
    bNode * brother = noRem->father->pointers[pos-1];
    int ocpNoRem = noRem->occupation;
    int ocpNoIrmao = brother->occupation;
    int i = ocpNoRem;

    while(i > 0) {
        noRem->keys[i] = noRem->keys[i-1];
        noRem->pointers[i+1] = noRem->pointers[i];
        i--;
    }
    noRem->pointers[i+1] = noRem->pointers[i];
    noRem->keys[i] = noRem->father->keys[pos - 1];
    noRem->occupation++;
    noRem->father->keys[pos - 1] = brother->keys[ocpNoIrmao-1];
    noRem->pointers[0] = brother->pointers[ocpNoIrmao];
    if (noRem->pointers[0] != NULL)
        noRem->pointers[0]->father = noRem;
    brother->occupation--;
    return;
}

void rightRotation(bNode * noRem, int pos) {
    bNode * brother = noRem->father->pointers[pos+1];
    int ocpNoIrmao = brother->occupation;
    int i = 0;

    noRem->keys[noRem->occupation] = noRem->father->keys[pos];
    noRem->occupation++;
    noRem->father->keys[pos] = brother->keys[0];
    noRem->pointers[noRem->occupation] = brother->pointers[0];
    if(!noRem->leaf) {
        noRem->pointers[noRem->occupation]->father = noRem;
    }

    while (i < ocpNoIrmao - 1) {
        brother->keys[i] = brother->keys[i + 1];
        brother->pointers[i] = brother->pointers[i+1];
        i++;
    }

    brother->pointers[i] = brother->pointers[i+1];
    brother->occupation--;

    return;
}

void merge(bTree *T, bNode * noEsq, bNode * noDir, int pos) {
    int i = noEsq->occupation;
    int j;
    bNode *father = noEsq->father;

    //UNIR OS DADOS NO noEsq
    noEsq->keys[i] = father->keys[pos];
    noEsq->occupation++;
    father->occupation--;
    i++;

    for (j=0; j < noDir->occupation; j++) {
        noEsq->keys[i] = noDir->keys[j];
        noEsq->pointers[i] = noDir->pointers[j];
        if(noEsq->pointers[i] != NULL)
            noEsq->pointers[i]->father = noEsq;
        noEsq->occupation++;
        i++;
    }

    noEsq->pointers[i] = noDir->pointers[j];
    if(noEsq->pointers[i] != NULL) {
        noEsq->pointers[i]->father = noEsq;
    }

    if ((father->occupation == 0) && (father == T->sentinel)) {
        T->sentinel = noEsq;
        noEsq->father = T->sentinel;
        noDir->father = T->sentinel;
        free(father);
        free(noDir);
        return;
    }

    //SHIFT NOS DTDOS DO father
    for (i = pos; i < noEsq->father->occupation; i++) {
        noEsq->father->keys[i] = noEsq->father->keys[i + 1];
        noEsq->father->pointers[i+1] = noEsq->father->pointers[i + 2];
    }

    free(noDir);
    return;
}

void removeElement(bTree *T, int key) {
    int i = 0;
    int fatherPosition = -1;
    int j;
    int min = (T->order - 1)/2;

    if(T->order % 2 != 0) {
        min = T->order / 2;
    }

    int find = 0;
    bNode *aux = T->sentinel;
    if (aux == NULL) { //arvore está vazia
        return;
    }

    while ((aux != NULL) && (find == 0) && (i<aux->occupation)) {
        if (key == aux->keys[i]) {
            find = 1;
            if (fatherPosition == -1) {
                fatherPosition = i;
                if (i == aux->occupation - 1)
                    fatherPosition++;
            }
        } else {
            if (key < aux->keys[i]) {
                aux = aux->pointers[i];
                fatherPosition = i;
                i = 0;
            } else {
                i = i + 1;
                if ((i == aux->occupation) && (key > aux->keys[i - 1])) {
                    aux = aux->pointers[i];
                    fatherPosition = i;
                    i = 0;
                }
            }
        }
    }

    if (aux == NULL) {
        return; //elemento não encontrado na árvore
    }

    //VERIFICTR SE O ELEMENTO É DE UM NÓ INTERMEDIÁRIO
    if (aux->leaf == 0) {
        //ENCONTRTR O SUCESSOR E RETLIZTR T CÓPIT
        bNode * sucessor = aux->pointers[i+1];
        fatherPosition = i + 1;
        while (sucessor->pointers[0] != NULL) {
            sucessor = sucessor->pointers[0];
            fatherPosition = 0;
        }
        //COPITR
        aux->keys[i] = sucessor->keys[0];
        aux = sucessor;
        i = 0;
    }
    //REMOVER O ELEMENTO DO NÓ
    j = aux->occupation - 1;
    while (i < j) {
        aux->keys[i] = aux->keys[i+1];
        i++;
    }
    aux->occupation--;
    //VERIFICTR SE DESBTLTNCEOU
    while(aux->occupation < min) {
        //CHTMTR ROTTÇÃO OU MERGE
        if ((aux != aux->father->pointers[0]) && (aux->father->pointers[fatherPosition - 1]->occupation > min)) { // pode emprestar do irmão da esquerda
            leftRotation(aux, fatherPosition);
        } else {
            if ((aux != aux->father->pointers[aux->father->occupation])  && (aux->father->pointers[fatherPosition + 1]->occupation > min)) { // pode emprestar do irmão da direita
                rightRotation(aux, fatherPosition);
            } else {
                if ((fatherPosition != 0) && (aux->father->pointers[fatherPosition - 1] != NULL)) {
                    fatherPosition--;
                    merge(T, aux->father->pointers[fatherPosition], aux, fatherPosition);
                } else
                    merge(T, aux, aux->father->pointers[fatherPosition + 1], fatherPosition);
                aux = aux->father;
                if (aux == T->sentinel) {
                    return;
                }
                //Caso contrário, encontra nova posição no father
                fatherPosition = 0;
                while ((fatherPosition < aux->father->occupation) && (key > aux->father->keys[fatherPosition])) {
                    fatherPosition++;
                }
            }
        }
    }
    return;
}

void printBTree(bNode *raiz) {
    int i;
    if (raiz == NULL)
        return;
    for (i = 0; i < raiz->occupation; i++) {
        printf("%d - %d\t", raiz->keys[i], raiz->father->keys[0]);
    }
    printf("\n");
    i = 0;
    while (i <= raiz->occupation) {
        printBTree(raiz->pointers[i]);
        i++;
    }
}

bNode* getRootNode(bTree *T) {
    return T->sentinel;
}

void insertFile(bTree *T, char fileName[]) {
    if(T == NULL) {
        printf("Árvore não encontrada!");
        exit(0);
    }

    FILE *pa;
    int num;
    pa = fopen(fileName, "r");

    while(!feof(pa)) {
        fscanf(pa, "%d\n", &num);
        insertElement(T, num);
    }

    fclose(pa);
}