#include <stdlib.h>
#include <stdio.h>
#include "../header/estruturas.h"
#include "../header/classificacao.h"


int main() {

    PMALHA malha = inicializar_malha();

    // DEBUG
    // ================================================================================
    // printf("\n--- MALHA LIDA ---\n");

    // printf("Vertices (%d):\n", malha->n_vertices);
    // for (int i = 0; i < malha->n_vertices; ++i) {
    //     printf("  V%d: (%d, %d)\n", i, malha->vertices[i].x, malha->vertices[i].y);
    // }

    // printf("Faces (%d):\n", malha->n_faces);
    // for (int i = 0; i < malha->n_faces; ++i) {
    //     printf("  F%d: ", i);
    //     for (int j = 0; j < malha->faces[i].n_vertices; ++j) {
    //         printf("%d ", malha->faces[i].indices[j]);
    //     }
    //     printf("\n");
    // }
    // ================================================================================
    

    int retorno = classifica(malha);
    switch (retorno) {
        case 1:
            printf("aberta\n");
            liberar_malha_entrada(malha);
            return 0;
        case 2:
            printf("não subdivisão planar\n");
            liberar_malha_entrada(malha);
            return 0;
        case 3:
            printf("superposta\n");
            liberar_malha_entrada(malha);
            return 0;
        default:
            break;
    }

    PDCEL dcel = gera_dcel(malha);
    printf("%d %d %d\n", dcel->n_vertices, dcel->n_arestas, dcel->n_faces);

    for (int i = 0; i < dcel->n_vertices; i++) {
        printf("%d %d %d\n", 
                dcel->vertices[i].x, 
                dcel->vertices[i].y, 
                dcel->vertices[i].semi_aresta);
    }
    
    for (int i = 0; i < dcel->n_faces; i++) {
        printf("%d\n", dcel->faces[i].semi_aresta);
    }
    
    int total_semi_arestas = dcel->n_arestas * 2;
    for (int i = 1; i <= total_semi_arestas; i++) {
        if (dcel->semi_arestas[i].origem != 0) { 
            printf("%d %d %d %d %d\n",
                    dcel->semi_arestas[i].origem,
                    dcel->semi_arestas[i].simetria,
                    dcel->semi_arestas[i].face_esquerda,
                    dcel->semi_arestas[i].proximo,
                    dcel->semi_arestas[i].anterior);
        }
    }

    liberar_malha_entrada(malha);
    liberar_dcel(dcel);
    
	return 0;
}

