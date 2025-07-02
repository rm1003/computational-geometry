#ifndef BSP_H
#define BSP_H

#include "estruturas.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define max(a,b) (a > b) ? a : b
#define min(a,b) (a < b) ? a : b

#define FRENTE 1
#define ATRAS -1
#define COPLANAR 0
#define CRUZANDO 2

int segmentoTrianguloCoplanar(PONTO A, PONTO B, PONTO V0, PONTO V1, PONTO V2, PONTO normal);

PLANO calcularPlano(TRIANGULO tri, PPONTO pontos);

PBSPNODO construirBSP(PTRIANGULO triangulos, ll num_triangulos, PPONTO pontos);

int intersecTriangulo(PONTO A, PONTO B, TRIANGULO tri, PPONTO pontos);

void buscaBSP(PBSPNODO nodo, PONTO a, PONTO b, PPONTO pontos, ll* marcado);
//void buscaBSP(PBSPNODO nodo, PONTO a, PONTO b, PPONTO pontos, ll* lista, ll* tamanho);

void processarSegmentos(PENTRADAS entrada);

#endif