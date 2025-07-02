#ifndef CLASSIFICACAO_H
#define CLASSIFICACAO_H

#include "estruturas.h"

#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)

// Produto vetorial
int crossProduct (VERTEX A, VERTEX B, VERTEX C);

// Funcao para verificar o sentido dos pontos
int orientation (VERTEX a, VERTEX b, VERTEX c);

// Funcao para verificar se o ponto r esta no segmento pq
int onSegment(VERTEX point, VERTEX p, VERTEX q);

// Funcao para verificar se dois segmentos (x1, y1) e (x2, y2) se intersectam
int segmentsIntersection (VERTEX x1, VERTEX y1, VERTEX x2, VERTEX y2);

// Funcao para verificar se um poligono eh "nao simples"
int notSimple (PPOLYGON p);

// Funcao para verificar se um poligono eh convexo
int isConvex (PPOLYGON p);

// Funcao para classificar o poligono
int polygonClassification (PPOLYGON p);

int pointInPolygon (PPOLYGON poly, VERTEX point);

#endif