#include "../header/estruturas.h"

// Cria a estrutura de dados principal
PDATA initData (int poly, int points)
{
    PDATA data = malloc (sizeof(DATA));
    if (!data) {
        perror("malloc");
        exit(1);
    }

    data->polygons = malloc (poly * sizeof(POLYGON));
    if (!data->polygons) {
        free(data);
        perror("malloc");
        exit(1);
    }

    data->points = malloc (points * sizeof(VERTEX));
    if (!data->points) {
        free(data->polygons);
        free(data);
        perror("malloc");
        exit(1);
    }

    data->classification = malloc (poly * sizeof(int));
    if (!data->classification) {
        free(data->polygons);
        free(data->points);
        free(data);
        perror("malloc");
        exit(1);
    }

    data->polygons_n = poly; // quantidade de poligonos
    data->points_n = points; // quantidade de pontos
    return data;
}

// Cria o poligono
void initPolygon (PPOLYGON p, int n) 
{   
    if (n < 3) {
        printf("Um poligono precisa de 3 lados.\n");
        exit(1);
    }
    p->n = n; // quantidade de vertices
    p->vertices = malloc (n * sizeof(VERTEX));
    if (!p->vertices) {
        perror("malloc");
        exit(1);
    }
    return;
}


// Libera memoria da estrutura principal
void freeData (PDATA d) 
{
    if (d) {
        for (int i = 0; i < d->polygons_n; ++i) {
            free(d->polygons[i].vertices);
        }
        free(d->polygons);
        free(d->points);
        free(d->classification);
        free(d);
    }
    return;
}


// Imprime o poligono
void printPolygon (PPOLYGON p)
{
    printf("Poligono com %d vertices:\n", p->n);
    for (int i = 0; i < p->n; ++i) {
        printf("{x: %d -- y: %d }\n", p->vertices[i].x, p->vertices[i].y);
    }
    return;
}

