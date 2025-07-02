#include "../header/estruturas.h"

static inline void initBsp (PENTRADAS entrada, ll n, ll t, ll l) 
{
    entrada->pontos = (PPONTO) malloc(n * sizeof(PONTO));
    entrada->triangulos = (PTRIANGULO) malloc(t * sizeof(TRIANGULO));
    entrada->segmentos = (PSEGMENTO) malloc(l * sizeof(SEGMENTO));
    entrada->num_pontos = n;
    entrada->num_triangulos = t;
    entrada->num_segmentos = l;
    entrada->arvoreBSP = NULL;
    return;
}

void liberarNodo(PBSPNODO nodo) 
{
    if (nodo == NULL) return;
    liberarNodo(nodo->frente);
    liberarNodo(nodo->atras);
    free(nodo->triangulos);
    free(nodo);
    return;
}

void freeBsp(PENTRADAS entrada) 
{
    liberarNodo(entrada->arvoreBSP);
    free(entrada->pontos);
    free(entrada->triangulos);
    free(entrada->segmentos);
    return;
}

void readInput (PENTRADAS entrada)
{
    ll n, t, l;
    
    if (scanf("%lld %lld %lld", &n, &t, &l) != 3) {
        fprintf(stderr, "Erro na leitura dos valores n, t, l\n");
        exit(EXIT_FAILURE);
    }
    initBsp (entrada, n, t, l);

    // le os pontos
    for (ll i = 0; i < n; ++i) {
        if (scanf("%lld %lld %lld", &entrada->pontos[i].x,
                                    &entrada->pontos[i].y,
                                    &entrada->pontos[i].z) != 3) {
            fprintf(stderr, "Erro na leitura do ponto %lld\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // le os triangulos
    for (ll i = 0; i < t; ++i) {
        if (scanf("%lld %lld %lld", &entrada->triangulos[i].vertices[0],
                                    &entrada->triangulos[i].vertices[1],
                                    &entrada->triangulos[i].vertices[2]) != 3) {
            fprintf(stderr, "Erro na leitura do ponto %lld\n", i);
            exit(EXIT_FAILURE);
        }
        entrada->triangulos[i].id = i + 1;
    }

    // le os segmentos
    for (ll i = 0; i < l; ++i) {
        if (scanf("%lld %lld %lld %lld %lld %lld",  &entrada->segmentos[i].a.x,
                                                    &entrada->segmentos[i].a.y,
                                                    &entrada->segmentos[i].a.z,
                                                    &entrada->segmentos[i].b.x,
                                                    &entrada->segmentos[i].b.y,
                                                    &entrada->segmentos[i].b.z) != 6) {
            fprintf(stderr, "Erro na leitura do ponto %lld\n", i);
            exit(EXIT_FAILURE);
        }
    }
    return;
}