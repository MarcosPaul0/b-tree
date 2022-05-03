#include <stdio.h>
#include <stdlib.h>

#include "btree.h"

int main() {
    arvoreB *A = criaArvore(5);
    /* insereElemento(A, 30);
    insereElemento(A, 20);
    insereElemento(A, 10);
    insereElemento(A, 5);
    insereElemento(A, 7);
    insereElemento(A, 18);
    insereElemento(A, 15);
    insereElemento(A, 12);
    insereElemento(A, 13);
    insereElemento(A, 14); */
    insereArquivo(A, "numeros.txt");
    imprimeArvore(retornaRaiz(A));
    printf("\n");
    removeElemento(A, 8);
    removeElemento(A, 2);
    removeElemento(A, 7);
    removeElemento(A, 20);
    imprimeArvore(retornaRaiz(A));
    return 0;
}