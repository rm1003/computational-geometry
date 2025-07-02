#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)

#define absf(a) ((a < 0) ? -a : a)

#define QTD_ARESTAS 1000

/* ============================================================================== */
// Input struct

// Arquivo de entrada:
// n e f (numero de vertices e numero de faces respectivamente)
// n linhas contendo as coordenadas de cada vertice xi e yi com i variando de 1 a n
// f linhas contendo os indices dos vertices de cada face, uma face por linha

struct ArestaInfo {
    int v1, v2;             // v1 < v2 para evitar duplicatas
    int contagem;           // Número de faces que usam esta aresta
    int indice_v1_v2;       // Indice da semi aresta v1 -> v2
    int indice_v2_v1;       // Indice da semi aresta v2 -> v1
};
typedef struct ArestaInfo ARESTA_INFO, *PARESTA_INFO;

struct TabelaArestas {
    PARESTA_INFO arestas;   // Array dinâmico de arestas
    int num_arestas;        // Total de arestas únicas
    int capacidade;         // Quantidade de arestas para controle de alocacao de memoria
};
typedef struct TabelaArestas TABELA_ARESTAS, *PTABELA_ARESTAS;


struct VerticeEntrada {
    int x, y;
};
typedef struct VerticeEntrada VERTICE_ENTRADA, *PVERTICE_ENTRADA;

struct FaceEntrada {
    int *indices;   // Array de indices dos vertices
    int n_vertices; // Numero de vertices na face
};
typedef struct FaceEntrada FACE_ENTRADA, *PFACE_ENTRADA;

struct Malha {
    PVERTICE_ENTRADA vertices;  // Array de vertices (indices de 1 a n)
    PFACE_ENTRADA faces;        // Array de faces
    PTABELA_ARESTAS tabela;     // Tabela contendo informacoes das arestas
    int n_vertices;             // Total de vértices (n)
    int n_faces;                // Total de faces (f)
};
typedef struct Malha MALHA, *PMALHA;
/* ============================================================================== */

/* ============================================================================== */

// DCEL struct
struct VerticeDCEL {
    int x, y;
    int semi_aresta;
};
typedef struct VerticeDCEL VERTICE_DCEL, *PVERTICE_DCEL;

struct FaceDCEL {
    int semi_aresta;
};
typedef struct FaceDCEL FACE_DCEL, *PFACE_DCEL;

struct SemiAresta {
    int origem;
    int simetria;
    int face_esquerda;
    int proximo;
    int anterior;
};
typedef struct SemiAresta SEMI_ARESTA, *PSEMI_ARESTA;

struct dcel {
    PVERTICE_DCEL vertices;     // Array de vertices
    PFACE_DCEL faces;           // Array de faces
    PSEMI_ARESTA semi_arestas;  // Array de semi-arestas
    int n_vertices;             // n
    int n_arestas;              // m
    int n_faces;                // f 
};
typedef struct dcel DCEL, *PDCEL;
/* ============================================================================== */

// Init da malha
PMALHA inicializar_malha ();

// Gera a DCEL
PDCEL gera_dcel (PMALHA malha);

// libera memoria da malha
void liberar_malha_entrada (PMALHA malha);

// libera memoria da dcel
void liberar_dcel (PDCEL DCEL);





#endif
