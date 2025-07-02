#include "../header/estruturas.h"

void contar_aresta(PMALHA malha,int a, int b, int *indice) 
{
    int v1 = min(a,b);
    int v2 = max(a,b);

    // Processa verificando se a aresta pertence a face [i] e incrementa o contador
    for (int i = 0; i < malha->tabela->num_arestas; i++) {
        if (malha->tabela->arestas[i].v1 == v1 && malha->tabela->arestas[i].v2 == v2) {
            malha->tabela->arestas[i].contagem++;
            return;
        }
    }

    if (malha->tabela->num_arestas == malha->tabela->capacidade) {
        malha->tabela->capacidade = malha->tabela->capacidade == 0 ? 1000 : malha->tabela->capacidade * 2;
        malha->tabela->arestas = realloc(malha->tabela->arestas, malha->tabela->capacidade * sizeof(ARESTA_INFO));
        if (!malha->tabela->arestas) {
            fprintf(stderr, "Erro de memória\n");
            exit(1);
        }
    }

    // Adicionando nova aresta na lista de arestas
    PARESTA_INFO nova = &malha->tabela->arestas[malha->tabela->num_arestas++];
    nova->v1 = v1;
    nova->v2 = v2;
    nova->contagem = 1;
    if (a < b) {
        nova->indice_v1_v2 = (*indice)++;
        nova->indice_v2_v1 = (*indice)++;
    } else {
        nova->indice_v2_v1 = (*indice)++;
        nova->indice_v1_v2 = (*indice)++;
    }

    // DEBUG
    // printf("%d %d = %d == %d =\n", nova->v1, nova->v2, nova->indice_v1_v2, nova->indice_v2_v1);
    return;
}

PMALHA inicializar_malha ()
{
    // Aloca memoria para estrutura da malha
    PMALHA malha = (PMALHA) malloc(sizeof(MALHA));
    if (!malha) {
        perror("Erro ao alocar malha");
        exit(1);
    } 
   
    // Leitura de quantidade de vertices e faces
    if (scanf("%d %d", &malha->n_vertices, &malha->n_faces) != 2) {
        fprintf(stderr, "Erro na leitura do número de vértices/faces\n");
        exit(1);
    }

    // Aloca memoria para os vertices da malha
    malha->vertices = (PVERTICE_ENTRADA) malloc(malha->n_vertices * sizeof(VERTICE_ENTRADA));
    if (!malha->vertices) {
        perror("Erro ao alocar vertices da malha.");
        free(malha);
        exit(1);
    }
    
    // Faz leitura dos vertices da malha
    for (int i = 0; i < malha->n_vertices; ++i) {
        if (scanf("%d %d", &malha->vertices[i].x, &malha->vertices[i].y) != 2) {
            perror("Erro na leitura de vértice");
            free(malha->vertices);
            free(malha);
            exit(1);
        }
    } 
    // Aloca memoria para as faces da malha
    malha->faces = (PFACE_ENTRADA) malloc(malha->n_faces * sizeof(FACE_ENTRADA));
    if (!malha->faces) {
        perror("Erro ao alocar faces");
        free(malha->vertices);
        free(malha);
        exit(1);
    }
   

    // Faz leitura das faces da malha
    getchar(); // tirar o \n do ultimo scanf
    char buffer[256];
    for (int i = 0; i < malha->n_faces; ++i) {
        malha->faces[i].indices = (int*) malloc(malha->n_vertices * sizeof(int));

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Erro na leitura da face\n");
            exit(1);
        }
        int qtd_indices = 0;

        char* token = strtok(buffer, " \n");
        while (token != NULL) {
            malha->faces[i].indices[qtd_indices] = atoi(token);
            qtd_indices++;
            token = strtok(NULL, " \n");
        }
        malha->faces[i].n_vertices = qtd_indices;
    }


    malha->tabela = malloc(sizeof(TABELA_ARESTAS));
    malha->tabela->arestas = NULL;
    malha->tabela->num_arestas = 0;
    malha->tabela->capacidade = 0;

    int indice = 1;
    for (int i = 0; i < malha->n_faces; i++) {
        int *v = malha->faces[i].indices;
        int n = malha->faces[i].n_vertices;
        for (int j = 0; j < n; j++) {
            int a = v[j];
            int b = v[(j + 1) % n];
            contar_aresta(malha, a, b, &indice);
        }
    }

    return malha;
}

// Gera a dcel inteira baseado nos dados ja obtidos na malha
PDCEL gera_dcel(PMALHA malha)
{
    PDCEL dcel = malloc(sizeof(DCEL));
    if (dcel == NULL) {
        perror("Erro ao alocar memória para DCEL\n");   
        return NULL;
    }

    // Armazena dados inicias
    dcel->n_vertices = malha->n_vertices;
    dcel->n_faces = malha->n_faces;
    dcel->n_arestas = malha->tabela->num_arestas;

    // Aloca estruturas da dcel
    dcel->vertices = malloc(sizeof(VERTICE_DCEL) * dcel->n_vertices);
    dcel->faces = malloc(sizeof(FACE_DCEL) * dcel->n_faces);
    dcel->semi_arestas = malloc(sizeof(SEMI_ARESTA) * ((dcel->n_arestas * 2) + 1));

    // Informacoes de x e y da malha
    for (int i = 0; i < malha->n_vertices; ++i) {
        dcel->vertices[i].x = malha->vertices[i].x;
        dcel->vertices[i].y = malha->vertices[i].y;
        // Inicializacao da semi-aresta que pertence ao vertice para nao ter erro de lixo de memoria
        dcel->vertices[i].semi_aresta = -1;
    }

    // Informacoes iniciais das semi-arestas baseado na tabela de arestas gerada
    ARESTA_INFO *aresta_info;
    int semi_aresta_direta, semi_aresta_inversa;
    for (int i = 0; i < malha->tabela->num_arestas; ++i) {
        aresta_info = &malha->tabela->arestas[i];
        semi_aresta_direta = aresta_info->indice_v1_v2;
        semi_aresta_inversa = aresta_info->indice_v2_v1;

        // Sao dois pois armazena dado dos dois vertices que pertence a aresta
        // Armazena origem 
        dcel->semi_arestas[semi_aresta_direta].origem = aresta_info->v1;
        dcel->semi_arestas[semi_aresta_inversa].origem = aresta_info->v2;
        // Armazena simetria
        dcel->semi_arestas[semi_aresta_direta].simetria = semi_aresta_inversa;
        dcel->semi_arestas[semi_aresta_inversa].simetria = semi_aresta_direta;

        // Inicializa outros campos para auxliar no armazenamento depois
        dcel->semi_arestas[semi_aresta_direta].face_esquerda = -1;
        dcel->semi_arestas[semi_aresta_inversa].face_esquerda = -1;
        dcel->semi_arestas[semi_aresta_direta].proximo = -1;
        dcel->semi_arestas[semi_aresta_inversa].proximo = -1;
        dcel->semi_arestas[semi_aresta_direta].anterior = -1;
        dcel->semi_arestas[semi_aresta_inversa].anterior = -1;
    }


    // Processa cada face para obter dados de cada semi-aresta
    FACE_ENTRADA *face_malha;
    int num_vertices_face;
    int primeira_semi_aresta;
    int semi_aresta_anterior;
    for (int face_idx = 0; face_idx < malha->n_faces; face_idx++) {
        face_malha = &malha->faces[face_idx];
        num_vertices_face = face_malha->n_vertices;

        // Inicializacao da variaiveis para fechar o ciclo de semi-arestas
        primeira_semi_aresta = -1;
        semi_aresta_anterior = -1;

        dcel->faces[face_idx].semi_aresta = -1;  // Inicializa face sem semi-aresta principal


        ARESTA_INFO *aresta_info;
        int vertice_atual;
        int vertice_proximo;
        for (int i = 0; i < num_vertices_face; ++i) {
            vertice_atual = face_malha->indices[i];
            vertice_proximo = face_malha->indices[(i + 1) % num_vertices_face];


            // Encontra a aresta correspondente na tabela de arestas
            aresta_info = NULL;
            for (int j = 0; j < malha->tabela->num_arestas; ++j) {
                if (malha->tabela->arestas[j].v1 == min(vertice_atual, vertice_proximo) &&
                    malha->tabela->arestas[j].v2 == max(vertice_atual, vertice_proximo)) {
                    aresta_info = &malha->tabela->arestas[j];
                    break;
                }
            }

            // Determina qual semi-aresta usar, direta ou inversa
            int semi_aresta_atual = (vertice_atual < vertice_proximo ? aresta_info->indice_v1_v2 : aresta_info->indice_v2_v1);
            
            // Associar a face com as semi-arestas
            dcel->semi_arestas[semi_aresta_atual].face_esquerda = face_idx + 1;

            // Se ainda nao definida, define a semi-aresta principal para a face atual
            if (dcel->faces[face_idx].semi_aresta < 0) {
                dcel->faces[face_idx].semi_aresta = semi_aresta_atual;
            }
            
            // Procurando pelo menor indice das semi-arestas que possuem origem em vertice_atual
            if (dcel->vertices[vertice_atual - 1].semi_aresta < 0 || 
                semi_aresta_atual < dcel->vertices[vertice_atual - 1].semi_aresta) {
                dcel->vertices[vertice_atual - 1].semi_aresta = semi_aresta_atual;
            }

            // Ligar as semi-arestas entre si (proximo e anterior)
            if (semi_aresta_anterior >= 0) {
                dcel->semi_arestas[semi_aresta_anterior].proximo = semi_aresta_atual;
                dcel->semi_arestas[semi_aresta_atual].anterior = semi_aresta_anterior;
            } else {
                primeira_semi_aresta = semi_aresta_atual;
            }
            semi_aresta_anterior = semi_aresta_atual;
        }

        // Fecha o ciclo da primeira semi-aresta com a ultima da face
        if (semi_aresta_anterior >= 0 && primeira_semi_aresta >= 0) {
            dcel->semi_arestas[semi_aresta_anterior].proximo = primeira_semi_aresta;
            dcel->semi_arestas[primeira_semi_aresta].anterior = semi_aresta_anterior;
        }
    }

    return dcel;
}

void liberar_malha_entrada (PMALHA malha)
{
    if (malha == NULL) return;
    for (int i = 0; i < malha->n_faces; ++i) {
        free(malha->faces[i].indices);
    }
    free(malha->tabela->arestas);
    free(malha->tabela);
    free(malha->faces);
    free(malha->vertices);
    free(malha);
}

void liberar_dcel(PDCEL dcel) 
{
    if (dcel == NULL) return;
    free(dcel->vertices);
    free(dcel->faces);
    free(dcel->semi_arestas);
    free(dcel);
}
