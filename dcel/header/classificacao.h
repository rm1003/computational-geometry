#ifndef CLASSIFICACAO_H
#define CLASSIFICACAO_H

#include "estruturas.h"

// Produto vetorial
int crossProduct (VERTICE_ENTRADA A, VERTICE_ENTRADA B, VERTICE_ENTRADA C);

// Funcao para verificar o sentido dos pontos
int orientation (VERTICE_ENTRADA a, VERTICE_ENTRADA b, VERTICE_ENTRADA c);

// Funcao para verificar se o ponto r esta no segmento pq
int onSegment (VERTICE_ENTRADA point, VERTICE_ENTRADA p, VERTICE_ENTRADA q);

// Funcao para verificar se dois segmentos (x1, y1) e (x2, y2) se intersectam
int segmentsIntersection (VERTICE_ENTRADA x1, VERTICE_ENTRADA y1, VERTICE_ENTRADA x2, VERTICE_ENTRADA y2);

// Classifica se a malha eh aberta
int malha_aberta (PMALHA malha);

// Classifica se a malha eh nao subdivisao planar
int malha_nao_subdivisao_planar (PMALHA malha);

// Classifica superpostas
int indice_igual (int i1, int i2, int j1, int j2);

// Verificacao de face anti horario
// Checa se o sentido da face, ou seja, se ela está no sentido horário ou anti-horário
// Retorna 1 para anti-horário, -1 para horário, e 0 para desconhecido (casos degenerados)
int face_anti_horaria(PFACE_ENTRADA face, PVERTICE_ENTRADA vertices);

int ray_casting(VERTICE_ENTRADA p, int indice_p, PFACE_ENTRADA face, PVERTICE_ENTRADA vertices);

int auto_intersecao (PFACE_ENTRADA face, PVERTICE_ENTRADA vertices);

int faces_intersectam (PFACE_ENTRADA f1, PFACE_ENTRADA f2, PVERTICE_ENTRADA vertices);

int face_superposta (PMALHA malha);

// Devolve qual classificacao deu
int classifica (PMALHA malha);

#endif
