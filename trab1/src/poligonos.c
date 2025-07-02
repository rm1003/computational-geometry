#include "../header/mainInclude.h"

int main () 
{   
    int m, n, i, j, polygon_vertex, points_x, points_y;
    PDATA data;
    
    
    if(scanf("%d %d", &m, &n) == 2) {
        data = initData(m, n);
    } else {
        return -1;
    }

    for (i = 0; i < m; ++i) {
        if (scanf("%d", &polygon_vertex) == 1) {
            initPolygon(&data->polygons[i], polygon_vertex);
        } else {
            return -1;
        }
        for (j = 0; j < polygon_vertex; ++j) {
            if (scanf("%d %d", &points_x, &points_y) == 2) {
                data->polygons[i].vertices[j].x = points_x;
                data->polygons[i].vertices[j].y = points_y;
            } else {
                return -1;
            }
        }
    }

    for (i = 0; i < n; ++i) {
        if (scanf("%d %d", &points_x, &points_y) == 2) {
            data->points[i].x = points_x;
            data->points[i].y = points_y;
        } else {
            return -1;
        }
    }

    for (i = 0; i < m; ++i) {
        // printPolygon(&data->polygons[i]);
        data->classification[i] = polygonClassification(&data->polygons[i]);
        if (data->classification[i] == 0) {
            printf("%d nao simples\n", i+1); // Nao simples
        } else if (data->classification[i] == 1) {
            printf("%d simples e convexo\n", i+1); // Simples e Convexo
        } else {
            printf("%d simples e nao convexo\n", i+1); // Simples e Nao Convexo
        }
    }

    for (i = 0; i < n; ++i) {
        int primeiro = 1;
        printf("%d:", i+1);
        for (j = 0; j < m; ++j) {
            //printf("-- %d - %d --\n", data->points[j].x, data->points[j].y);
            if (data->classification[j] != 0) {
                if (pointInPolygon(&data->polygons[j], data->points[i]) == 1) {
                    // printPolygon(&data->polygons[i]);
                    if (primeiro) {
                        primeiro = 0;
                        printf("%d", j+1);
                    } else {
                        printf(" %d", j+1);
                    }
                }
            }
        }
        printf("\n");
    }

    freeData(data);

    return 0;
}