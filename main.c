#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pessoa {
    char nome[30];
    char cpf[11];
} Pessoa;

typedef struct no {
    Pessoa pessoa;
    struct no* prox;
} No;

typedef struct vertice {
    No* cab;
    Pessoa pessoa;
} Vertice;

typedef struct grafo {
    int numVertices;
    Vertice * vertices;
} Grafo;

// ===========================CABEÇALHOS========================================
Grafo * criarGrafo(int num);
No* criarNo(Pessoa p);
Grafo* adicionaVertice(Grafo* g, char nome[30], char cpf[11]);
Grafo* adicionarAresta(Grafo* g, char nome1[30], char nome2[30]);
void imprimirCaminhoRecursivo(Grafo *grafo, int verticeAtual, int verticeDestino, char *caminho[], int nivel);
void imprimirCaminho(Grafo *grafo, char *nomeOrigem, char *nomeDestino);
void indicarAmigos(Grafo* g, char nome1[30], char nome2[30]);
Grafo* inserir(char cpf[11],char nome[30], Grafo* g);
int busca(Grafo* g, char nome[30]);
void remover(Grafo* g, char nome[30]);
// =============================================================================+

int main() {

    int opcao;          // Variavel para escolher opcoes do MENU
    int retorno;        // Retorno

    Grafo *Grafo;
    Grafo = criarGrafo(10);

    do
    {
        printf("\n1. Inserir Vertice\n2. Inserir Aresta\n3. Buscar Usuario\n");                  // Opcoes MENU
        printf("4. Rota para outra pessoa\n5. Indicacoes\n6. Remover\n7. Sair\n");               // Opcoes MENU
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
            case 1: // Inserir Vértice
            {
                char nome[30];
                printf("Insira o nome do novo usuario: ");
                scanf("%s", nome);
                getchar();

                char cpf[11];
                printf("Insira o cpf do novo usuario: ");
                scanf("%s", cpf);
                getchar();

                Grafo = inserir(cpf, nome, Grafo);

                break;
            }

            case 2: // Adicionar Aresta
            {
                char nome1[30], nome2[30];
                printf("Informe o nome de quem realizara a conexao: ");
                scanf("%s", nome1);
                printf("Informe o nome de quem sera conectado: ");
                scanf("%s", nome2);

                if (strcmp(nome1, nome2) == 0) {
                    printf("ERRO! No Grafo nao pode ter laco (1° cpf = 2° cpf).\n");
                }

                else {
                    Grafo = adicionarAresta(Grafo, nome1, nome2);
                }

                break;
            }

            case 3: // Buscar
            {
                char nome[30];
                printf("Insira um nome para buscar: ");
                scanf("%s", nome);
                getchar();

                retorno = busca(Grafo, nome);

                if (retorno == -1) {
                    printf("'%s' nao foi achado!\n", nome);
                }

                else {
                    printf("\n%s possui cpf: %s\n", nome, Grafo->vertices[retorno].pessoa.cpf);
                }

                break;
            }

            case 4: // Rota de pessoa A para pessoa B
            {
                char nome1[30], nome2[30]; // vai ser usada para pegar o vertice 1 e 2 informada pelo usuario
                printf("Informe o nome origem da rota: ");
                scanf("%s", nome1);
                printf("Informe o destino destino da rota: ");
                scanf("%s", nome2);

                if (strcmp(nome1, nome2) == 0) {
                    printf("ERRO! No Grafo nao pode ter laco (1° cpf = 2° cpf).\n");
                }

                else {
                    imprimirCaminho(Grafo, nome1, nome2);
                }

                break;
            }

            case 5: // Indicações
            {
                char nome1[30], nome2[30];
                printf("Informe o nome de quem deseja uma indicacao: ");
                scanf("%s", nome1);
                printf("Atraves de qual amigo voce deseja essa indicacao: ");
                scanf("%s", nome2);

                indicarAmigos(Grafo, nome1, nome2);

                break;
            }

            case 6: // Remover
            {
                char nome[30];
                printf("Informe o nome do usuario a ser removido: ");
                scanf("%s", nome);

                remover(Grafo, nome);

                break;
            }

            case 7: // Sair
            {
                printf("Obrigado por usar a Rede Social!");
                break;
            }

            default:
            {
                printf("Opcao Invalida !!!\n");
                break;
            }
        }
    } while (opcao != 7);           // Se Opção for 7 ele sai do laço e "fecha" o programa

    free(Grafo);

    return 0;
}

Grafo * criarGrafo(int num){
    Grafo* g = (Grafo*) malloc(sizeof(Grafo));
    g->numVertices = num;
    g->vertices = (Vertice *) malloc(num * sizeof(Vertice));
    for(int i = 0;i<num;i++) {
        g->vertices[i].cab = NULL;

        strcpy(g->vertices[i].pessoa.cpf,"-1");
        strcpy(g->vertices[i].pessoa.nome,"-1");
    }
    return g;
}

No* criarNo(Pessoa p){
    No* novo = (No*) malloc(sizeof(No));
    novo->pessoa = p;
    novo->prox = NULL;
    return novo;
}

//adiciona um vertice num novo espaço vazio
Grafo* adicionaVertice(Grafo* g, char nome[30], char cpf[11]){

    Pessoa p;
    strcpy(p.cpf,cpf);
    strcpy(p.nome, nome);
    No* no = (No*) malloc(sizeof(No));
    for(int i = 0; i<g->numVertices;i++){
        if(strcmp(g->vertices[i].pessoa.cpf,"-1") == 0){
            g->vertices[i].cab = no;
            g->vertices[i].pessoa = p;
            return g;
        }
    }
    printf("A rede social esta cheia");
    return g;
}

//Conectar duas pessoas
Grafo* adicionarAresta(Grafo* g, char nome1[30], char nome2[30]){
    int indice1 = busca(g, nome1);
    int indice2 = busca(g,nome2);

    Pessoa a, b;

    if (strcmp(nome1, nome2) != 0) {
        if (indice1 != -1 && indice2 != -1) {
            a = g->vertices[indice1].pessoa;
            b = g->vertices[indice2].pessoa;

            No *novo = criarNo(b);
            novo->prox = g->vertices[indice1].cab;
            g->vertices[indice1].cab = novo;

            No *novo2 = criarNo(a);
            novo2->prox = g->vertices[indice2].cab;
            g->vertices[indice2].cab = novo2;

            printf("Conexao cadastrada com sucesso!");
        }
        else{
            printf("Ambos precisam estar cadastrados");}
    }
    else {
        printf("Os nomes são iguais!");
    }

    return g;
}

// Função auxiliar para imprimir o caminho recursivamente
void imprimirCaminhoRecursivo(Grafo *grafo, int verticeAtual, int verticeDestino, char *caminho[], int nivel) {
    if (verticeAtual == verticeDestino) {
        // Chegamos ao vértice de destino, imprima o caminho
        for (int i = 0; i < nivel; i++) {
            printf("%s -> ", caminho[i]);
        }
        printf("%s\n", caminho[nivel]);
        return;
    }

    No *temp = grafo->vertices[verticeAtual].cab;
    while (temp != NULL) {
        int indice = -1;
        for (int i = 0; i < grafo->numVertices; i++) {
            if (strcmp(grafo->vertices[i].pessoa.cpf, temp->pessoa.cpf) == 0) {
                indice = i;
                break;
            }
        }
        if (indice != -1) {
            // Evitar ciclos no caminho
            if (caminho[indice] == NULL) {
                caminho[indice] = temp->pessoa.nome;
                imprimirCaminhoRecursivo(grafo, indice, verticeDestino, caminho, nivel + 1);
                caminho[indice] = NULL;
            }
        }
        temp = temp->prox;
    }
}

void imprimirCaminho(Grafo *grafo, char *nomeOrigem, char *nomeDestino) {
    // Procurar os índices dos vértices no grafo
    int indiceOrigem = busca(grafo, nomeOrigem);
    int indiceDestino = busca(grafo, nomeDestino);

    // Verificar se ambos os vértices foram encontrados
    if (indiceOrigem != -1 && indiceDestino != -1) {
        char *caminho[grafo->numVertices];

        for (int i = 0; i < grafo->numVertices; i++) {
            caminho[i] = NULL;
        }

        caminho[indiceOrigem] = grafo->vertices[indiceOrigem].pessoa.nome;
        printf("\nCaminho de %s para %s:\n", grafo->vertices[indiceOrigem].pessoa.nome, grafo->vertices[indiceDestino].pessoa.nome);
        imprimirCaminhoRecursivo(grafo, indiceOrigem, indiceDestino, caminho, 0);
    } else {
        printf("Vértices de origem ou destino não encontrados no grafo.\n");
    }
}

void indicarAmigos(Grafo* g, char nome1[30], char nome2[30]) {
    int indice1, indice2;
    int flag = -1;
    No *n;
    indice1 = busca(g,nome1);
    indice2 = busca(g,nome2);
    if (strcmp(nome1, nome2) == 0) {
        printf("Voce precisa passar dois nomes diferentes");
        return;
    }
    if(indice1 != -1 && indice2 != -1) {
        for (n = g->vertices[indice1].cab; n != NULL; n = n->prox) {
            if (strcmp(n->pessoa.nome, nome2) == 0) {
                flag = 0;
                break;
            }
        }
        if(flag == -1){printf("%s nao pode indicar ninguem para %s pois nao eh seu amigo", nome2, nome1);}
        No *temp = g->vertices[indice2].cab;
        while (temp != NULL) {
            for (n = g->vertices[indice1].cab; n != NULL; n = n->prox) {
                if (strcmp(n->pessoa.cpf, temp->pessoa.cpf) != 0) {
                    printf("\n%s eh uma boa indicacao para %s\n", temp->pessoa.nome, nome1);
                    return;
                }
            }
            temp = temp->prox;
        }
    }
    else{
        printf("Ambos precisam estar cadastrados na rede");
    }
}

Grafo* inserir(char cpf[11],char nome[30], Grafo* g){
    g = adicionaVertice(g, nome, cpf);
    return g;
}

int busca(Grafo* g, char nome[30]){
    for(int i=0;i<g->numVertices;i++){
        if(strcmp(g->vertices[i].pessoa.nome,nome) == 0){
            return i;
        }
    }
    return -1;
}

void remover(Grafo* g, char nome[30]){
    int indice = busca(g,nome);
    No* delete;
    No* n;
    if(indice!=-1){
        for(int i=0;i<g->numVertices;i++) {
            if(n!=NULL){
                n = g->vertices[i].cab;
                if(strcmp(n->pessoa.nome,nome)==0){
                    delete = n;
                    n = n->prox;
                    free(delete);
                }
                else {
                    while (n != NULL) {
                        if (strcmp(n->prox->pessoa.nome, nome) == 0) {
                            delete = n->prox;
                            n->prox = n->prox->prox;
                            free(delete);
                        }
                    }
                }}
        }
        strcpy(g->vertices[indice].pessoa.cpf, "-1");
        strcpy(g->vertices[indice].pessoa.nome, "-1");
    }
    else{
        printf("Esse nome nao esta cadastrado");
    }
}

