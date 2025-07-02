#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdlib.h>
#include <stdio.h>

struct vertex {
    int x, y;
};
typedef struct vertex VERTEX, *PVERTEX;

struct polygon {
    PVERTEX vertices;
    int n; // quantidade de pontos
};
typedef struct polygon POLYGON, *PPOLYGON;

struct data {
    PPOLYGON polygons;
    PVERTEX points;
    int *classification;
    int polygons_n , points_n;
};
typedef struct data DATA, *PDATA;

// Cria a estrutura de dados principal
PDATA initData (int poly, int points);

// Cria o poligono
void initPolygon (PPOLYGON p, int n);

// Libera memoria da estrutura principal
void freeData (PDATA d); 

// Imprime o poligono
void printPolygon (PPOLYGON p);

#endif