#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* -------------------- Estruturas Expandidas -------------- */

typedef struct {
    int HP;
    int ataque;
    int defesa;
    int ataque_especial;
    int defesa_especial;
    int velocidade;
    int total;
} STATS;

typedef struct {
    char nome[30];
    char tipo[15];
    int poder;
    int precisao;
    char categoria[10];
} MOVIMENTO;

typedef struct evolucao {
    char nome_evolucao[50];
    int nivel_necessario;
    char item_necessario[30];
    char metodo[20];
    struct evolucao *proxima;
} EVOLUCAO;

typedef struct pokemon {
    int numero_pokedex;
    char nome[50];
    char especie[50];
    char descricao[200];
    float altura;
    float peso;
    char genero;
    char tipo_primario[15];
    char tipo_secundario[15];
    char habilidade[30];
    char habilidade_oculta[30];
    STATS stats_base;
    int taxa_captura;
    char grupo_ovo[30];
    int passos_eclodir;
    MOVIMENTO movimentos[4];
    int num_movimentos;
    EVOLUCAO *evolucoes;
} POKEMON;

typedef struct nodo {
    POKEMON *pPokemon;
    struct nodo *pProximo;
    struct nodo *pAnterior;
} NODO;

NODO *pInicio = NULL;
NODO *pFim = NULL;

const char *TIPOS_POKEMON[] = {
    "Normal", "Fogo", "Água", "Planta", "Elétrico", 
    "Gelo", "Lutador", "Venenoso", "Terrestre", "Voador",
    "Psíquico", "Inseto", "Pedra", "Fantasma", "Dragão",
    "Aço", "Sombrio", "Fada"
};
const int NUM_TIPOS = 18;

const char *CATEGORIAS_MOVIMENTO[] = {
    "Físico", "Especial", "Status"
};

const char *GRUPOS_OVO[] = {
    "Monstro", "Água1", "Água2", "Água3", "Inseto", "Dragão",
    "Campo", "Fada", "Planta", "Humanoide", "Mineral", "Amorfo",
    "Voador", "Desconhecido"
};

/* --------------- Protótipos de Funções --------- */

int str_case_cmp(const char *s1, const char *s2);
void Pausa(void);
void LimpaBuffer(void);
void MenuPrincipal(void);
NODO *CriaNovoNodo(void);
void InsereNoFim(NODO *novoNodo);
void RemovePokemonPorNome(void);
void LiberaLista(void);
void ListaPokemon(void);
void ListaPokemonDetalhado(void);
void SalvaLista(void);
void CarregaLista(void);
void MostrarTiposPokemon(void);
int SelecionarTipo(void);
void CalcularTotalStats(STATS *stats);
void ColetarStats(STATS *stats);
void ColetarMovimentos(POKEMON *pPokemon);
void ColetaDadosPokemon(POKEMON *pPokemon);
void LiberarEvolucoes(EVOLUCAO *evolucao);

/* ------------------ Função Principal -------------- */

int main(void) {
    CarregaLista();
    MenuPrincipal();
    LiberaLista();
    return 0;
}

/* -------------------- Funções Auxiliares ------------------- */

int str_case_cmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return (tolower((unsigned char)*s1) - tolower((unsigned char)*s2));
        }
        s1++;
        s2++;
    }
    return (tolower((unsigned char)*s1) - tolower((unsigned char)*s2));
}

void LimpaBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void Pausa(void) {
    printf("\n Pressione ENTER para continuar...");
    LimpaBuffer();
}

void CalcularTotalStats(STATS *stats) {
    stats->total = stats->HP + stats->ataque + stats->defesa + 
                   stats->ataque_especial + stats->defesa_especial + 
                   stats->velocidade;
}

void MostrarTiposPokemon(void) {
    for (int i = 0; i < NUM_TIPOS; i++) {
        printf(" %d. %s\n", i + 1, TIPOS_POKEMON[i]);
    }
}

int SelecionarTipo(void) {
    MostrarTiposPokemon();
    int tipo;
    printf(" Escolha o tipo: ");
    scanf("%d", &tipo);
    LimpaBuffer();
    return (tipo > 0 && tipo <= NUM_TIPOS) ? tipo - 1 : 0;
}

/* -------------------- Menu Principal Expandido ------------------- */

void MenuPrincipal(void) {
    int opcao = 0;
    
    do {
        system("clear || cls"); 
        printf("\n#################################################\n");
        printf("#           POKEDEX PRO                         #\n");
        printf("#################################################\n");
        printf(" 1. Cadastrar Novo Pokemon\n");
        printf(" 2. Listar Todos os Pokemons (Resumo)\n");
        printf(" 3. Listar Pokemon Detalhado\n");
        printf(" 4. Remover Pokemon por Nome\n");
        printf(" 5. Salvar Lista no Arquivo\n");
        printf(" 6. Carregar Lista do Arquivo\n");
        printf(" 7. Estatisticas da Pokedex\n");
        printf(" 8. Sair\n");
        printf("-------------------------------------------------\n");
        printf(" Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        LimpaBuffer();

        switch (opcao) {
            case 1:
                InsereNoFim(CriaNovoNodo()); 
                Pausa();
                break;
            case 2:
                ListaPokemon();
                Pausa();
                break;
            case 3:
                ListaPokemonDetalhado();
                Pausa();
                break;
            case 4:
                RemovePokemonPorNome();
                Pausa();
                break;
            case 5:
                SalvaLista();
                Pausa();
                break;
            case 6:
                LiberaLista();
                CarregaLista(); 
                Pausa();
                break;
            case 7:
                printf("\n Estatisticas em desenvolvimento...\n");
                Pausa();
                break;
            case 8:
                printf("\n Fim do Pokedex Pro - Ate logo!\n");
                break;
            default:
                printf("\n Opcao invalida! Tente novamente.\n");
                Pausa();
                break;
        }
    } while (opcao != 8);
}

/* ------------------- Funções de Alocação -------------------- */

NODO *CriaNovoNodo(void) {
    NODO *pNodoAtual = (NODO *)malloc(sizeof(NODO));
    POKEMON *pPokemonAtual = (POKEMON *)malloc(sizeof(POKEMON));

    if (pNodoAtual == NULL || pPokemonAtual == NULL) {
        printf("\n Erro: Memoria insuficiente.\n");
        if (pNodoAtual) free(pNodoAtual);
        if (pPokemonAtual) free(pPokemonAtual);
        return NULL;
    }

    memset(pPokemonAtual, 0, sizeof(POKEMON));
    pPokemonAtual->evolucoes = NULL;
    pPokemonAtual->num_movimentos = 0;

    pNodoAtual->pPokemon = pPokemonAtual;
    pNodoAtual->pProximo = NULL;
    pNodoAtual->pAnterior = NULL;

    ColetaDadosPokemon(pNodoAtual->pPokemon);
    
    return pNodoAtual;
}

void InsereNoFim(NODO *novoNodo) {
    if (novoNodo == NULL) return;
    
    if (pInicio == NULL) {
        pInicio = novoNodo;
        pFim = novoNodo;
    } else {
        pFim->pProximo = novoNodo;
        novoNodo->pAnterior = pFim;
        pFim = novoNodo;
    }
}

/* ------------------- Coleta de Dados Expandida -------------------- */

void ColetarStats(STATS *stats) {
    printf("\n--- STATS BASE ---\n");
    
    printf(" HP: ");
    scanf("%d", &stats->HP);
    printf(" Ataque: ");
    scanf("%d", &stats->ataque);
    printf(" Defesa: ");
    scanf("%d", &stats->defesa);
    printf(" Ataque Especial: ");
    scanf("%d", &stats->ataque_especial);
    printf(" Defesa Especial: ");
    scanf("%d", &stats->defesa_especial);
    printf(" Velocidade: ");
    scanf("%d", &stats->velocidade);
    
    CalcularTotalStats(stats);
    LimpaBuffer();
}

void ColetarMovimentos(POKEMON *pPokemon) {
    printf("\n--- MOVIMENTOS (Max 4) ---\n");
    
    for (int i = 0; i < 4; i++) {
        char opcao;
        printf("\n Adicionar movimento %d? (s/n): ", i + 1);
        scanf(" %c", &opcao);
        LimpaBuffer();
        
        if (opcao != 's' && opcao != 'S') break;
        
        printf(" Nome do movimento: ");
        fgets(pPokemon->movimentos[i].nome, 30, stdin);
        pPokemon->movimentos[i].nome[strcspn(pPokemon->movimentos[i].nome, "\n")] = 0;
        
        printf(" Tipo: ");
        fgets(pPokemon->movimentos[i].tipo, 15, stdin);
        pPokemon->movimentos[i].tipo[strcspn(pPokemon->movimentos[i].tipo, "\n")] = 0;
        
        printf(" Poder: ");
        scanf("%d", &pPokemon->movimentos[i].poder);
        printf(" Precisao: ");
        scanf("%d", &pPokemon->movimentos[i].precisao);
        LimpaBuffer();
        
        printf(" Categoria (Fisico/Especial/Status): ");
        fgets(pPokemon->movimentos[i].categoria, 10, stdin);
        pPokemon->movimentos[i].categoria[strcspn(pPokemon->movimentos[i].categoria, "\n")] = 0;
        
        pPokemon->num_movimentos++;
    }
}

void ColetaDadosPokemon(POKEMON *pPokemon) {
    system("clear || cls");
    printf("\n=== CADASTRO COMPLETO DE POKEMON ===\n");
    
    printf("\n--- DADOS BASICOS ---\n");
    printf(" Numero na Pokedex: ");
    scanf("%d", &pPokemon->numero_pokedex);
    LimpaBuffer();
    
    printf(" Nome: ");
    fgets(pPokemon->nome, 50, stdin);
    pPokemon->nome[strcspn(pPokemon->nome, "\n")] = 0;
    
    printf(" Especie: ");
    fgets(pPokemon->especie, 50, stdin);
    pPokemon->especie[strcspn(pPokemon->especie, "\n")] = 0;
    
    printf(" Descricao: ");
    fgets(pPokemon->descricao, 200, stdin);
    pPokemon->descricao[strcspn(pPokemon->descricao, "\n")] = 0;
    
    printf("\n--- CARACTERISTICAS FISICAS ---\n");
    printf(" Altura (m): ");
    scanf("%f", &pPokemon->altura);
    printf(" Peso (kg): ");
    scanf("%f", &pPokemon->peso);
    LimpaBuffer();
    
    printf(" Genero (M/F/N): ");
    scanf(" %c", &pPokemon->genero);
    LimpaBuffer();
    
    printf("\n--- TIPOS ---\n");
    printf(" Tipo Primario:\n");
    int tipo = SelecionarTipo();
    strcpy(pPokemon->tipo_primario, TIPOS_POKEMON[tipo]);
    
    printf(" Possui tipo secundario? (s/n): ");
    char opcao;
    scanf(" %c", &opcao);
    LimpaBuffer();
    if (opcao == 's' || opcao == 'S') {
        printf(" Tipo Secundario:\n");
        tipo = SelecionarTipo();
        strcpy(pPokemon->tipo_secundario, TIPOS_POKEMON[tipo]);
    } else {
        strcpy(pPokemon->tipo_secundario, "Nenhum");
    }
    
    printf("\n--- HABILIDADES ---\n");
    printf(" Habilidade: ");
    fgets(pPokemon->habilidade, 30, stdin);
    pPokemon->habilidade[strcspn(pPokemon->habilidade, "\n")] = 0;
    
    printf(" Habilidade Oculta: ");
    fgets(pPokemon->habilidade_oculta, 30, stdin);
    pPokemon->habilidade_oculta[strcspn(pPokemon->habilidade_oculta, "\n")] = 0;
    
    ColetarStats(&pPokemon->stats_base);
    
    printf("\n Taxa de Captura: ");
    scanf("%d", &pPokemon->taxa_captura);
    LimpaBuffer();
    
    printf(" Grupo de Ovo: ");
    fgets(pPokemon->grupo_ovo, 30, stdin);
    pPokemon->grupo_ovo[strcspn(pPokemon->grupo_ovo, "\n")] = 0;
    
    printf(" Passos para eclodir: ");
    scanf("%d", &pPokemon->passos_eclodir);
    LimpaBuffer();
    
    ColetarMovimentos(pPokemon);
    
    printf("\n Pokemon cadastrado com sucesso!\n");
}

/* -------------------- Funções de Visualização Expandidas ------------------- */

void ListaPokemon(void) {
    NODO *pAtual = pInicio;
    
    system("clear || cls");
    printf("\n=================================================\n");
    printf("           LISTA DE POKEMONS              \n");
    printf("=================================================\n");
    
    if (pInicio == NULL) {
        printf("\n A Pokedex esta vazia.\n");
        return;
    }
    
    int contador = 1;
    while (pAtual != NULL) {
        POKEMON *p = pAtual->pPokemon;
        printf("\n#%03d - %s\n", p->numero_pokedex, p->nome);
        printf(" Especie: %s | Tipo: %s", p->especie, p->tipo_primario);
        if (strcmp(p->tipo_secundario, "Nenhum") != 0) {
            printf("/%s", p->tipo_secundario);
        }
        printf("\n Stats Total: %d | Movimentos: %d\n", p->stats_base.total, p->num_movimentos);
        printf("-------------------------------------------------\n");
        pAtual = pAtual->pProximo;
        contador++;
    }
    printf(" Total: %d Pokemons\n", contador - 1);
}

void ListaPokemonDetalhado(void) {
    printf("\n Visualizacao detalhada em desenvolvimento...\n");
}

void SalvaLista(void) {
    printf("\n Salvamento em desenvolvimento...\n");
}

void CarregaLista(void) {
    // Função vazia para evitar erros
}

void RemovePokemonPorNome(void) {
    printf("\n Remocao em desenvolvimento...\n");
}

/* ---------------------- Libera Memória Expandida ---------------------------- */

void LiberarEvolucoes(EVOLUCAO *evolucao) {
    while (evolucao != NULL) {
        EVOLUCAO *temp = evolucao;
        evolucao = evolucao->proxima;
        free(temp);
    }
}

void LiberaLista(void) {
    NODO *pAtual = pInicio;
    NODO *pTemp;
    
    while (pAtual != NULL) {
        pTemp = pAtual->pProximo;
        if (pAtual->pPokemon) {
            LiberarEvolucoes(pAtual->pPokemon->evolucoes);
            free(pAtual->pPokemon);
        }
        free(pAtual);
        pAtual = pTemp;
    }
    
    pInicio = NULL;
    pFim = NULL;
}
