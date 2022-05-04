#include <stdio.h>
#include <stdlib.h>

#include "btree.h"

int main() {
    bTree *T = createBTree(5);
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
    insertFile(T, "numbers.txt");
    printBTree(getRootNode(T));
    printf("\n");
    removeElement(T, 8);
    removeElement(T, 2);
    removeElement(T, 7);
    removeElement(T, 20);
    printBTree(getRootNode(T));
    return 0;
}