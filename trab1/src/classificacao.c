#include "../header/classificacao.h"


// Produto vetorial
/*
    A(x1,y1) B(x2,y2) C(x3,y3)
    AB = (x2-x1, y2-y1)
    BC = (x3-x2, y3-y2)
    AB x BC = ABx.BCy - ABy.BCx
*/
int crossProduct (VERTEX A, VERTEX B, VERTEX C) 
{
    return ((B.x-A.x)*(C.y-B.y)) - ((B.y-A.y)*(C.x-B.x));
}

// Funcao para verificar o sentido dos pontos
int orientation (VERTEX a, VERTEX b, VERTEX c) 
{
    int value;
    value = crossProduct(a, b, c); // Produto vetorial
    if (value > 0) return -1; // Anti-horario
    if (value < 0) return 1; // Horario
    return 0; // Colinear
}

// Funcao para verificar se o ponto r esta na regiao delimitada
// retangularmente pelo segmento dado por p e q
int onSegment(VERTEX point, VERTEX p, VERTEX q) 
{
    if (point.x <= max(p.x, q.x) && point.x >= min(p.x, q.x) &&
        point.y <= max(p.y, q.y) && point.y >= min(p.y, q.y)) {
        return 1;
    }
    return 0;
}

// Funcao para verificar se dois segmentos (x1, y1) e (x2, y2) se intersectam
int segmentsIntersection (VERTEX x1, VERTEX y1, VERTEX x2, VERTEX y2)
{
    int o1, o2, o3, o4;
    o1 = orientation(x1, y1, x2);
    o2 = orientation(x1, y1, y2);
    o3 = orientation(x2, y2, x1);
    o4 = orientation(x2, y2, y1);

    // Se orientation(x1, y1, x2) != orientation(x1, y1, y2) e 
    // orientation(x2, y2, x1) != orientation(x2, y2, y1)
    // Significa que eles se cruzam em um ponto
    if (o1 != o2 && o3 != o4) return 1;

    // Casos colineares
    if (o1 == 0 && onSegment(x2, x1, y1)) return 1;
    if (o2 == 0 && onSegment(y2, x1, y1)) return 1;
    if (o3 == 0 && onSegment(x1, x2, y2)) return 1;
    if (o4 == 0 && onSegment(y1, x2, y2)) return 1;

    return 0;
}

// Funcao para verificar se um poligono eh "nao simples"
int notSimple (PPOLYGON p)
{
    int n = p->n;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 2; j < n; ++j) {
            if (i == 0 && j == (n - 1)) continue; // Ignorando primeiro e ultima
            // (j+1) % n é para garantir o fechamento do poligono e não ultrapassar do ultimo
            if (segmentsIntersection(p->vertices[i], p->vertices[i+1], p->vertices[j], p->vertices[(j+1) % n])) {
                return 1;
            }
        }
    }
    return 0;
}

// Funcao para verificar se um poligono eh convexo
int isConvex (PPOLYGON p)
{
    int sense, n, ori;
    sense = 0;
    n = p->n;
    for (int i = 0; i < n; ++i) {
        // Verificando a orientacao dos segmentos consecutivos
        ori = orientation(p->vertices[i], p->vertices[(i + 1) % n], p->vertices[(i + 2) % n]);
        if (ori != 0) {
            if (sense == 0) sense = ori;
            else if (sense != ori) return 0; // Se a orientacao for diferente do anterior
        }
    }
    return 1;
}

// Funcao para classificar o poligono
int polygonClassification (PPOLYGON p) 
{
    if (notSimple(p)) {
        return 0; // Nao simples
    } else if (isConvex(p)) {
        return 1; // Simples e Convexo
    } else {
        return 2; // Simples e Nao Convexo
    }
}

// Ray Casting
int pointInPolygon (PPOLYGON poly, VERTEX point)
{
    VERTEX currentVertex, nextVertex;
    int count, n;
    count = 0;
    n = poly->n;

    for (int i = 0; i < n; ++i) {
        currentVertex = poly->vertices[i];
        nextVertex = poly->vertices[(i+1) % n];

        // Se o ponto eh um vertice do poligono
        if (point.x == currentVertex.x && point.y == currentVertex.y) {
            return 1;
        }

        // O ponto esta exatamente em cima da aresta
        if (onSegment(point, currentVertex, nextVertex) &&
            ((nextVertex.y - currentVertex.y) * (point.x - currentVertex.x) ==
             (nextVertex.x - currentVertex.x) * (point.y - currentVertex.y))) {
            return 1;
        }

        if ((currentVertex.y > point.y) != (nextVertex.y > point.y)) {
            double x_intersect = currentVertex.x + 
                                (point.y - currentVertex.y) * (nextVertex.x - currentVertex.x) /
                                (nextVertex.y - currentVertex.y);
            if (point.x < x_intersect) {
                count++;
            }

        }
    }
    return count % 2; // Impar = dentro, Par = fora
}
