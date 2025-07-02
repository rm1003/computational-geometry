#include "../header/estruturas.h"
#include "../header/bsp.h"

static inline void print(PENTRADAS entrada)
{
    printf("%lld %lld %lld\n", entrada->num_pontos, entrada->num_triangulos, entrada->num_segmentos);
    
    for (ll i = 0; i < entrada->num_pontos; i++) {
        printf("%lld %lld %lld\n", entrada->pontos[i].x, entrada->pontos[i].y, entrada->pontos[i].z);
    }
    
    for (ll i = 0; i < entrada->num_triangulos; i++) {
        printf("%lld %lld %lld\n", 
               entrada->triangulos[i].vertices[0],
               entrada->triangulos[i].vertices[1],
               entrada->triangulos[i].vertices[2]);
    }
    
    for (ll i = 0; i < entrada->num_segmentos; i++) {
        printf("%lld %lld %lld %lld %lld %lld\n",
               entrada->segmentos[i].a.x, entrada->segmentos[i].a.y, entrada->segmentos[i].a.z,
               entrada->segmentos[i].b.x, entrada->segmentos[i].b.y, entrada->segmentos[i].b.z);
    }
    return;
}

static inline void imprimirBSP(PBSPNODO nodo, int nivel) 
{
    if (nodo == NULL) return;

    for (int i = 0; i < nivel; i++) printf("  ");

    printf("Nível %d: Plano (%lld, %lld, %lld)x + %lld = 0\n", 
           nivel, 
           nodo->planos.normal.x,
           nodo->planos.normal.y,
           nodo->planos.normal.z,
           nodo->planos.distancia);

    for (int i = 0; i < nodo->qtd_triangulos; i++) {
        for (int j = 0; j < nivel; j++) printf("  ");
        printf("Triângulo %lld: %lld %lld %lld\n",
               nodo->triangulos[i].id,
               nodo->triangulos[i].vertices[0],
               nodo->triangulos[i].vertices[1],
               nodo->triangulos[i].vertices[2]);
    }

    imprimirBSP(nodo->frente, nivel + 1);
    imprimirBSP(nodo->atras, nivel + 1);
}

int main() {
    srand(time(NULL));
    ENTRADAS entrada;
    
    readInput(&entrada);
    
    // Verifica se a leitura foi correta imprimindo de volta
    // printf("\nVerificação da leitura:\n");
    // printf("=======================\n");
    // print(&entrada);
    // printf("=======================\n");

    entrada.arvoreBSP = construirBSP(entrada.triangulos, entrada.num_triangulos, entrada.pontos);
    // printf("\nEstrutura da BSP:\n");
    // printf("=================\n");
    // imprimirBSP(entrada.arvoreBSP, 0);
    // printf("=================\n");

    processarSegmentos(&entrada);
    
    freeBsp(&entrada);
    return 0;
}