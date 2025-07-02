#include "../header/classificacao.h"
#include "../header/estruturas.h"

int crossProduct (VERTICE_ENTRADA A, VERTICE_ENTRADA B, VERTICE_ENTRADA C)
{
    return ((B.x-A.x)*(C.y-B.y)) - ((B.y-A.y)*(C.x-B.x));
}

int orientation (VERTICE_ENTRADA a, VERTICE_ENTRADA b, VERTICE_ENTRADA c)
{
    int value = crossProduct(a, b, c);
    if (value > 0) return -1; // Anti-horário
    if (value < 0) return 1;  // Horário
    return 0;                 // Colinear
}

int onSegment (VERTICE_ENTRADA point, VERTICE_ENTRADA p, VERTICE_ENTRADA q)
{
    if (point.x <= max(p.x, q.x) && point.x >= min(p.x, q.x) &&
        point.y <= max(p.y, q.y) && point.y >= min(p.y, q.y)) {
        return 1;
    }
    return 0;
}

int segmentsIntersection (VERTICE_ENTRADA x1, VERTICE_ENTRADA y1, VERTICE_ENTRADA x2, VERTICE_ENTRADA y2)
{
    int o1 = orientation(x1, y1, x2);
    int o2 = orientation(x1, y1, y2);
    int o3 = orientation(x2, y2, x1);
    int o4 = orientation(x2, y2, y1);

    if (o1 != o2 && o3 != o4) return 1;

    if (o1 == 0 && onSegment(x2, x1, y1)) return 1;
    if (o2 == 0 && onSegment(y2, x1, y1)) return 1;
    if (o3 == 0 && onSegment(x1, x2, y2)) return 1;
    if (o4 == 0 && onSegment(y1, x2, y2)) return 1;

    return 0;
}

// =====================================================================================
// Teste malha aberta
int malha_aberta (PMALHA malha) 
{
    for (int i = 0; i < malha->tabela->num_arestas; i++) {
        if (malha->tabela->arestas[i].contagem == 1) {
            return 1;
        }
    }
    return 0;
}

// =====================================================================================
// Teste subdivisao planar 
int malha_nao_subdivisao_planar (PMALHA malha) 
{
    for (int i = 0; i < malha->tabela->num_arestas; i++) {
        if (malha->tabela->arestas[i].contagem > 2) {
            return 1;
        }
    }
    return 0;
}

// =====================================================================================
// Teste superposicao
int aresta_igual (VERTICE_ENTRADA a1, VERTICE_ENTRADA a2, VERTICE_ENTRADA b1, VERTICE_ENTRADA b2)
{
    return ((a1.x == b1.x && a1.y == b1.y && a2.x == b2.x && a2.y == b2.y) ||
            (a1.x == b2.x && a1.y == b2.y && a2.x == b1.x && a2.y == b1.y));
}

// Ignorando ordem
int indice_igual (int i1, int i2, int j1, int j2) 
{
    return (i1==j1 && i2==j2) || (i1==j2 && i2==j1);
    //return (i1==j1) || (i2==j2) || (i1==j2) || (i2==j1);
}

// Ignorando ordem
int indice_igual_detalhado (int i1, int i2, int j1, int j2) 
{
    //return (i1==j1 && i2==j2) || (i1==j2 && i2==j1);
    return (i1==j1) || (i2==j2) || (i1==j2) || (i2==j1);
}

// Verificacao de face anti horario
// Checa se o sentido da face, ou seja, se ela está no sentido horário ou anti-horário
// Retorna 1 para anti-horário, -1 para horário, e 0 para desconhecido (casos degenerados)
int face_anti_horaria(PFACE_ENTRADA face, PVERTICE_ENTRADA vertices)
{
    if (face->n_vertices < 3) {
        return 0;
    }

    VERTICE_ENTRADA v1, v2;
    int area = 0;
    for (int i = 0; i < face->n_vertices; ++i) {
        v1 = vertices[face->indices[i] - 1];
        v2 = vertices[face->indices[(i + 1) % face->n_vertices] - 1];
        area += (v2.x - v1.x) * (v2.y + v1.y);
    }

    // area negativa, entao eh anti-horario
    return (area < 0) - (area > 0);
}

int ray_casting(VERTICE_ENTRADA p, int p_indice, PFACE_ENTRADA face, PVERTICE_ENTRADA vertices) 
{
    VERTICE_ENTRADA a, b;
    int contador = 0;
    int n = face->n_vertices;
    
    for (int i = 0; i < n; ++i) {
        // Se o ponto p faz parte dos vértices da face, então ignora ele
        if (face->indices[i] == p_indice) {
            return 0;
        }
    }
    
    for (int i = 0; i < n; ++i) {
        a = vertices[face->indices[i] - 1];
        b = vertices[face->indices[(i + 1) % n] - 1];
        
        // Evita comparar o vértice p com ele próprio
        if (face->indices[i] == p_indice || face->indices[(i+1) % n] == p_indice)
            continue;
        
        // Testa se o raio horizontal para a direita cruza 
        if (orientation(a, b, p) == 0 && onSegment(p, a, b))
            return 0;

        if ( ((a.y <= p.y && p.y < b.y) || (b.y <= p.y && p.y < a.y)) ) {
            double xint = a.x + (double)(p.y - a.y)*(b.x - a.x)/(double)(b.y - a.y);
            if (xint >= p.x) {
                contador++;
            }
        }
    }
    return (contador % 2) == 1;
}

int auto_intersecao (PFACE_ENTRADA face, PVERTICE_ENTRADA vertices)
{
    int n = face->n_vertices;
    VERTICE_ENTRADA a1, a2, b1, b2;

    for (int i = 0; i < n; ++i) {
        a1 = vertices[face->indices[i] - 1];
        a2 = vertices[face->indices[(i + 1) % n] - 1];

        for (int j = i + 1; j < n; ++j) {
            // Evita comparar arestas consecutivas ou que compartilham vértice
            if ((i + 1) % n == j || i == (j + 1) % n) continue;

            b1 = vertices[face->indices[j] - 1];
            b2 = vertices[face->indices[(j + 1) % n] - 1];

            if (segmentsIntersection(a1, a2, b1, b2)) return 1;
        }
    }
    return 0;
}

int faces_intersectam(PFACE_ENTRADA faceA, PFACE_ENTRADA faceB, PVERTICE_ENTRADA vertices) 
{
    
    VERTICE_ENTRADA pontoA1, pontoA2;
    int n = faceA->n_vertices;
    // Verifica intersecao entre as arestas nao compartilhadas
    for (int i = 0; i < n; ++i) {
        pontoA1 = vertices[faceA->indices[i] - 1];
        pontoA2 = vertices[faceA->indices[(i + 1) % n] - 1];
        
        int m = faceB->n_vertices;
        VERTICE_ENTRADA pontoB1, pontoB2;
        for (int j = 0; j < m; ++j) {
            // Ignora arestas compartilhadas
            if (indice_igual_detalhado(faceA->indices[i], faceA->indices[(i + 1) % n], faceB->indices[j], faceB->indices[(j+1) % m] )) 
                continue;

            pontoB1 = vertices[faceB->indices[j] - 1];
            pontoB2 = vertices[faceB->indices[(j+1) % m] - 1];
            
            if (segmentsIntersection(pontoA1, pontoA2, pontoB1, pontoB2)) {
                return 1;
            }
        }
    }

    // Se as faces possuem orientações diferentes, ignora a sobreposição porque nesse caso não faz sentido comparar
    // Geralmente isso ocorre ao comparar uma face externa (da região fora da malha) com uma face interna ()de um polígono dentro da malha)
    ////if (face_anti_horaria(faceA, vertices) == face_anti_horaria(faceB, vertices)) {
        
        // Verifica se vertices de uma face esta dentro da outra face
        for (int i = 0; i < faceA->n_vertices; ++i) {
            VERTICE_ENTRADA ponto = vertices[faceA->indices[i] - 1];
            
            if (ray_casting(ponto, faceA->indices[i], faceB, vertices))
            {
                if (face_anti_horaria(faceB, vertices) > 0) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
        
        // Sentido contrario
        for (int i = 0; i < faceB->n_vertices; ++i) {
            VERTICE_ENTRADA ponto = vertices[faceB->indices[i] - 1];
            
            if (ray_casting(ponto, faceB->indices[i], faceA, vertices))
            {
                if (face_anti_horaria(faceA, vertices) > 0) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
    ////}

    if (face_anti_horaria(faceA, vertices) > 0 || face_anti_horaria(faceB, vertices) > 0) {
        return 0;
    } else {
        return 1;
    }
}

int face_superposta (PMALHA malha)
{
    // Verificar auto-interseções
    for (int i = 0; i < malha->n_faces; i++) {
        if (auto_intersecao(&malha->faces[i], malha->vertices)) {
            // printf("Auto-intersec\n");
            return 1;
        }
    }

    // Verificar interseção entre diferentes faces
    for (int i = 0; i < malha->n_faces; i++) {
        for (int j = i + 1; j < malha->n_faces; j++) {
            if (faces_intersectam(&malha->faces[i], &malha->faces[j], malha->vertices)) {
                return 1;
            }
        }
    }

    return 0;
}

// =====================================================================================

int classifica (PMALHA malha)
{
    if (malha_aberta(malha)) {
        return 1;
    } else if (malha_nao_subdivisao_planar(malha)) {
        return 2;
    } else if (face_superposta(malha)) {
        return 3;
    }
    return 0;
}
