#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdlib.h>
#include <stdio.h>

#define NUM_VERTICES 3

typedef long long ll;
// Estruturas basicas
// ===============================================
typedef struct ponto PONTO, *PPONTO;
typedef struct triangulo TRIANGULO, *PTRIANGULO;
typedef struct segmento SEGMENTO, *PSEGMENTO;

struct ponto {
    ll x, y, z;
};

struct triangulo {
    ll vertices[NUM_VERTICES];
    ll id;
};

struct segmento{
    PONTO a, b;
};
// ===============================================


// Estrutura para a BSP
// ===============================================
typedef struct bspnodo BSPNODO, *PBSPNODO;
typedef struct plano PLANO, *PPLANO;
typedef struct entradas ENTRADAS, *PENTRADAS;

struct plano {
    PONTO normal;
    ll distancia;
};

struct bspnodo {
    PLANO planos;
    PTRIANGULO triangulos;
    ll qtd_triangulos;
    PBSPNODO frente, atras; // nodos
};

struct entradas {
    PPONTO pontos;
    ll num_pontos;

    PTRIANGULO triangulos;
    ll num_triangulos;

    PSEGMENTO segmentos;
    ll num_segmentos;

    PBSPNODO arvoreBSP; // raiz da arvore
};
// ===============================================

// Libera os nodos da bsp
void liberarNodo(PBSPNODO nodo);

// Libera bsp
void freeBsp (PENTRADAS entrada);

// Leitura de entradas
void readInput (PENTRADAS entrada);

#endif