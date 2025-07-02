#include "../header/bsp.h"


static inline int comparar(const void* a, const void* b) 
{
    ll arg1 = *(const ll*)a;
    ll arg2 = *(const ll*)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

static inline ll min3(ll a, ll b, ll c) 
{
    return (a < b ? (a < c ? a : c) : (b < c ? b : c));
}
static inline ll max3(ll a, ll b, ll c) 
{
    return (a > b ? (a > c ? a : c) : (b > c ? b : c));
}


static inline int pontoDentroTriangulo2D(ll px, ll py, ll ax, ll ay, ll bx, ll by, ll cx, ll cy) 
{
    ll d1 = (px - bx) * (ay - by) - (ax - bx) * (py - by);
    ll d2 = (px - cx) * (by - cy) - (bx - cx) * (py - cy);
    ll d3 = (px - ax) * (cy - ay) - (cx - ax) * (py - ay);

    int tem_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    int tem_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(tem_neg && tem_pos);
}

static inline int orientacao(ll x1, ll y1, ll x2, ll y2, ll x3, ll y3) 
{
    ll val = (y2 - y1) * (x3 - x1) - (x2 - x1) * (y3 - y1);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

static inline int emSegmento(ll x1, ll y1, ll x2, ll y2, ll x3, ll y3) 
{
    return (x2 <= max(x1, x3) && x2 >= min(x1, x3) &&
            y2 <= max(y1, y3) && y2 >= min(y1, y3));
}

static inline int segmentosCruzam2D(ll p1x, ll p1y, ll q1x, ll q1y, ll p2x, ll p2y, ll q2x, ll q2y) 
{
    int o1 = orientacao(p1x, p1y, q1x, q1y, p2x, p2y);
    int o2 = orientacao(p1x, p1y, q1x, q1y, q2x, q2y);
    int o3 = orientacao(p2x, p2y, q2x, q2y, p1x, p1y);
    int o4 = orientacao(p2x, p2y, q2x, q2y, q1x, q1y);

    if (o1 != o2 && o3 != o4) return 1;

    if (o1 == 0 && emSegmento(p1x, p1y, p2x, p2y, q1x, q1y)) return 1;
    if (o2 == 0 && emSegmento(p1x, p1y, q2x, q2y, q1x, q1y)) return 1;
    if (o3 == 0 && emSegmento(p2x, p2y, p1x, p1y, q2x, q2y)) return 1;
    if (o4 == 0 && emSegmento(p2x, p2y, q1x, q1y, q2x, q2y)) return 1;

    return 0;
}

int segmentoTrianguloCoplanar(PONTO A, PONTO B, PONTO V0, PONTO V1, PONTO V2, PONTO normal) 
{
    int eixo = 0;
    if (llabs(normal.y) > llabs(normal.x)) eixo = 1;
    if (llabs(normal.z) > llabs(normal.x) && llabs(normal.z) > llabs(normal.y)) eixo = 2;

    ll Ax, Ay, Bx, By;
    ll V0x, V0y, V1x, V1y, V2x, V2y;

    if (eixo == 0) {
        Ax = A.y; Ay = A.z; Bx = B.y; By = B.z;
        V0x = V0.y; V0y = V0.z; V1x = V1.y; V1y = V1.z; V2x = V2.y; V2y = V2.z;
    } else if (eixo == 1) {
        Ax = A.x; Ay = A.z; Bx = B.x; By = B.z;
        V0x = V0.x; V0y = V0.z; V1x = V1.x; V1y = V1.z; V2x = V2.x; V2y = V2.z;
    } else {
        Ax = A.x; Ay = A.y; Bx = B.x; By = B.y;
        V0x = V0.x; V0y = V0.y; V1x = V1.x; V1y = V1.y; V2x = V2.x; V2y = V2.y;
    }

    ll triMinX = min3(V0.x, V1.x, V2.x);
    ll triMaxX = max3(V0.x, V1.x, V2.x);
    ll triMinY = min3(V0.y, V1.y, V2.y);
    ll triMaxY = max3(V0.y, V1.y, V2.y);
    ll triMinZ = min3(V0.z, V1.z, V2.z);
    ll triMaxZ = max3(V0.z, V1.z, V2.z);

    ll segMinX = A.x < B.x ? A.x : B.x;
    ll segMaxX = A.x > B.x ? A.x : B.x;
    ll segMinY = A.y < B.y ? A.y : B.y;
    ll segMaxY = A.y > B.y ? A.y : B.y;
    ll segMinZ = A.z < B.z ? A.z : B.z;
    ll segMaxZ = A.z > B.z ? A.z : B.z;

    if ((segMaxX < triMinX) || (segMinX > triMaxX) ||
        (segMaxY < triMinY) || (segMinY > triMaxY) ||
        (segMaxZ < triMinZ) || (segMinZ > triMaxZ)) {
        return 0;
    }

    ll area2 = (V1x - V0x) * (V2y - V0y) - (V2x - V0x) * (V1y - V0y);
    if (area2 == 0) {
        if (segmentosCruzam2D(Ax, Ay, Bx, By, V0x, V0y, V1x, V1y)) return 1;
        if (segmentosCruzam2D(Ax, Ay, Bx, By, V1x, V1y, V2x, V2y)) return 1;
        if (segmentosCruzam2D(Ax, Ay, Bx, By, V2x, V2y, V0x, V0y)) return 1;
        return 0;
    }
    if (pontoDentroTriangulo2D(Ax, Ay, V0x, V0y, V1x, V1y, V2x, V2y)) return 1;
    if (pontoDentroTriangulo2D(Bx, By, V0x, V0y, V1x, V1y, V2x, V2y)) return 1;
    if (segmentosCruzam2D(Ax, Ay, Bx, By, V0x, V0y, V1x, V1y)) return 1;
    if (segmentosCruzam2D(Ax, Ay, Bx, By, V1x, V1y, V2x, V2y)) return 1;
    if (segmentosCruzam2D(Ax, Ay, Bx, By, V2x, V2y, V0x, V0y)) return 1;

    return 0;
}


static inline ll classificarPonto(PLANO plano, PONTO p) 
{
    ll distancia =  plano.normal.x * p.x +
                    plano.normal.y * p.y + 
                    plano.normal.z * p.z + 
                    plano.distancia;

    if (distancia == 0) return COPLANAR;
    return (distancia > 0) ? FRENTE : ATRAS;
}

static inline ll classificarTriangulo(PLANO plano, TRIANGULO triangulo, PPONTO pontos) 
{
    ll frente = 0, atras = 0;

    for (ll i = 0; i < NUM_VERTICES; ++i) {
        PONTO ponto = pontos[triangulo.vertices[i] - 1];
        ll lado = classificarPonto(plano, ponto);

        if (lado == FRENTE) frente++;
        else if (lado == ATRAS) atras++;
    }

    if (frente == 0 && atras == 0) return COPLANAR;
    if (frente > 0 && atras > 0) return CRUZANDO;
    return (frente > 0) ? FRENTE : ATRAS;
}


PLANO calcularPlano(TRIANGULO tri, PPONTO pontos) 
{
    PLANO pl;
    PONTO p1 = pontos[tri.vertices[0] - 1];
    PONTO p2 = pontos[tri.vertices[1] - 1];
    PONTO p3 = pontos[tri.vertices[2] - 1];

    PONTO v1 = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z};
    PONTO v2 = {p3.x - p1.x, p3.y - p1.y, p3.z - p1.z};

    // produto vetorial v1 × v2
    pl.normal.x = v1.y * v2.z - v1.z * v2.y;
    pl.normal.y = v1.z * v2.x - v1.x * v2.z;
    pl.normal.z = v1.x * v2.y - v1.y * v2.x;

    // distancia da origem: - (N · P)
    pl.distancia = -(pl.normal.x * p1.x + pl.normal.y * p1.y + pl.normal.z * p1.z);

    return pl;
}

PBSPNODO construirBSP(PTRIANGULO triangulos, ll num_triangulos, PPONTO pontos)
{
    if (num_triangulos == 0) return NULL;

    PBSPNODO nodo = (PBSPNODO)malloc(sizeof(BSPNODO));
    nodo->frente = nodo->atras = NULL;

    ll indice_aleatorio = rand() % num_triangulos;
    TRIANGULO tri_divisao = triangulos[indice_aleatorio];
    nodo->planos = calcularPlano(tri_divisao, pontos);

    // listas auxiliares
    PTRIANGULO coplanares = (PTRIANGULO)malloc(num_triangulos * sizeof(TRIANGULO));
    PTRIANGULO frente = (PTRIANGULO)malloc(num_triangulos * sizeof(TRIANGULO));
    PTRIANGULO atras = (PTRIANGULO)malloc(num_triangulos * sizeof(TRIANGULO));
    
    ll num_coplanares = 0;
    ll num_frente = 0;
    ll num_atras = 0;

    coplanares[num_coplanares++] = tri_divisao;

    for (ll i = 0; i < num_triangulos; ++i) {
        if (i == indice_aleatorio) continue;

        ll classificacao = classificarTriangulo(nodo->planos, triangulos[i], pontos);
        switch (classificacao) {
            case COPLANAR: 
                coplanares[num_coplanares++] = triangulos[i]; 
                break;
            case FRENTE: 
                frente[num_frente++] = triangulos[i]; 
                break;
            case ATRAS: 
                atras[num_atras++] = triangulos[i]; 
                break;
            case CRUZANDO:
                frente[num_frente++] = triangulos[i];
                atras[num_atras++] = triangulos[i];
                break;
        }
    }

    nodo->triangulos = (PTRIANGULO)realloc(coplanares, num_coplanares * sizeof(TRIANGULO));
    nodo->qtd_triangulos = num_coplanares;
    nodo->frente = construirBSP(frente, num_frente, pontos);
    nodo->atras = construirBSP(atras, num_atras, pontos);

    free(frente);
    free(atras);
    return nodo;
}

int intersecTriangulo(PONTO A, PONTO B, TRIANGULO tri, PPONTO pontos) 
{
    PONTO V0 = pontos[tri.vertices[0] - 1];
    PONTO V1 = pontos[tri.vertices[1] - 1];
    PONTO V2 = pontos[tri.vertices[2] - 1];

    // Primeiro verifica se algum ponto do segmento coincide com um vertice
    if ((A.x == V0.x && A.y == V0.y && A.z == V0.z) ||
        (A.x == V1.x && A.y == V1.y && A.z == V1.z) ||
        (A.x == V2.x && A.y == V2.y && A.z == V2.z) ||
        (B.x == V0.x && B.y == V0.y && B.z == V0.z) ||
        (B.x == V1.x && B.y == V1.y && B.z == V1.z) ||
        (B.x == V2.x && B.y == V2.y && B.z == V2.z)) {
        return 1;
    }

    PONTO edge1 = {V1.x - V0.x, V1.y - V0.y, V1.z - V0.z};
    PONTO edge2 = {V2.x - V0.x, V2.y - V0.y, V2.z - V0.z};
    PONTO dir = {B.x - A.x, B.y - A.y, B.z - A.z};

    PONTO pvec = {
        dir.y * edge2.z - dir.z * edge2.y,
        dir.z * edge2.x - dir.x * edge2.z,
        dir.x * edge2.y - dir.y * edge2.x
    };

    ll det = edge1.x * pvec.x + edge1.y * pvec.y + edge1.z * pvec.z;

    if (det == 0) {
        PONTO normal = {
            edge1.y * edge2.z - edge1.z * edge2.y,
            edge1.z * edge2.x - edge1.x * edge2.z,
            edge1.x * edge2.y - edge1.y * edge2.x
        };
        ll planoA = normal.x * (A.x - V0.x) + normal.y * (A.y - V0.y) + normal.z * (A.z - V0.z);
        ll planoB = normal.x * (B.x - V0.x) + normal.y * (B.y - V0.y) + normal.z * (B.z - V0.z);

        if (planoA != 0 || planoB != 0) return 0;
        return segmentoTrianguloCoplanar(A, B, V0, V1, V2, normal);
    }

    PONTO tvec = {A.x - V0.x, A.y - V0.y, A.z - V0.z};
    ll u_num = tvec.x * pvec.x + tvec.y * pvec.y + tvec.z * pvec.z;
    
    if (det > 0) {
        if (u_num < 0 || u_num > det) return 0;
    } else {
        if (u_num > 0 || u_num < det) return 0;
    }

    PONTO qvec = {
        tvec.y * edge1.z - tvec.z * edge1.y,
        tvec.z * edge1.x - tvec.x * edge1.z,
        tvec.x * edge1.y - tvec.y * edge1.x
    };

    ll v_num = dir.x * qvec.x + dir.y * qvec.y + dir.z * qvec.z;
    if (det > 0) {
        if (v_num < 0 || v_num > det) return 0;
    } else {
        if (v_num > 0 || v_num < det) return 0;
    }

    if (det > 0) {
        if (u_num + v_num > det) return 0;
    } else {
        if (u_num + v_num < det) return 0;
    }

    ll t_num = edge2.x * qvec.x + edge2.y * qvec.y + edge2.z * qvec.z;
    if (det > 0) {
        if (t_num < 0 || t_num > det) return 0;
    } else {
        if (t_num > 0 || t_num < det) return 0;
    }

    return 1;
}

static inline ll classificarSegmento(PLANO plano, PONTO a, PONTO b) 
{
    ll lado_a = classificarPonto(plano, a);
    ll lado_b = classificarPonto(plano, b);

    if (lado_a == lado_b) return lado_a;
    return CRUZANDO;
}


static inline void verificaTriangulosNoNodo(PBSPNODO nodo, PONTO a, PONTO b, PPONTO pontos, ll* marcado) 
{
    for (ll i = 0; i < nodo->qtd_triangulos; ++i) {
        TRIANGULO triangulo = nodo->triangulos[i];
        if (intersecTriangulo(a, b, triangulo, pontos)) {
            marcado[triangulo.id] = 1;
        }
    }
}

// Busca recursiva na BSP
void buscaBSP(PBSPNODO nodo, PONTO a, PONTO b, PPONTO pontos, ll* marcado) 
{
    if (nodo == NULL) return;
    verificaTriangulosNoNodo(nodo, a, b, pontos, marcado);
    ll classificacao = classificarSegmento(nodo->planos, a, b);
    if (classificacao == FRENTE) {
        buscaBSP(nodo->frente, a, b, pontos, marcado);
    } else if (classificacao == ATRAS) {
        buscaBSP(nodo->atras, a, b, pontos, marcado);
    } else if (classificacao == CRUZANDO || classificacao == COPLANAR) {
        buscaBSP(nodo->frente, a, b, pontos, marcado);
        buscaBSP(nodo->atras, a, b, pontos, marcado);
    }
}

void processarSegmentos(PENTRADAS entrada) 
{
    for (ll i = 0; i < entrada->num_segmentos; ++i) {
        SEGMENTO seg = entrada->segmentos[i];
        ll* marcado = (ll*)calloc(entrada->num_triangulos + 1, sizeof(ll));
        buscaBSP(entrada->arvoreBSP, seg.a, seg.b, entrada->pontos, marcado);

        ll total = 0;
        for (ll j = 1; j <= entrada->num_triangulos; j++) {
            if (marcado[j]) total++;
        }

        ll* lista = (ll*)malloc(total * sizeof(ll));
        ll pos = 0;
        for (ll j = 1; j <= entrada->num_triangulos; j++) {
            if (marcado[j]) lista[pos++] = j;
        }

        qsort(lista, total, sizeof(ll), comparar);

        printf("%lld", total);
        for (ll j = 0; j < total; j++) {
            printf(" %lld", lista[j]);
        }

        if (i != entrada->num_segmentos - 1) {
            printf("\n");
        }

        free(lista);
        free(marcado);
    }
}