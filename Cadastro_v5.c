

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*--------------------estruturas --------------*/
typedef struct pessoa
{
    char nome[50];
    int idade;
    
} PESSOA;

typedef struct node
{
    PESSOA *pPessoa;
    struct node* pProximo;
    struct node* pAnterior;

} NODO;

/*---------variáveis globais ---------------------*/

NODO* pInicio = NULL;
NODO* pFim = NULL;
NODO* pAux = NULL;

/*--------------- protótipos de funçãoes ---------*/
void MainMenu();
void CadastraMenu(void);
NODO * NovaPessoa(void);
void CadastraPessoa(NODO *pNodo);
void ListaPessoas(void);
void LiberaLista(void);
void SalvaLista(char *nomeArquivo);
void CarregaLista(char *nomeArquivo);
NODO * BuscaPessoa(char *nome);
void EditaCadastro(void);

/*------------------ funçao principal--------------*/
int main()
{
    MainMenu();
    
    return 0;
}
/*--------------------Menu Principal-----------------------------------------*/
void MainMenu()
{
    int opcao;
    char nomeBusca[50];

    printf("------------------ Main Menu ------------------\n");
    printf("1 - Cadastrar nova pessoa\n");
    printf("2 - Listar pessoas\n");
    printf("3 - Buscar pessoa por nome\n");
    printf("4 - Editar cadastro\n");
    printf("5 - Salvar lista em arquivo\n");
    printf("6 - Carregar lista de arquivo\n");
    printf("7 - Sair do cadastro");
    printf("\n---------------------------------------------\n");
    printf("\n");
    printf("Digite sua opção: ");
    scanf("%d", &opcao);
    getchar(); // Limpa o buffer do teclado

    switch(opcao)
    {
        case 1:
                //Cadastra Nova pessoa
                CadastraMenu();
                break;
        case 2:
                //Lista pessoas cadastradas
                ListaPessoas();
                break;
        case 3:
                //Busca pessoa por nome
                printf("Digite o nome a buscar: ");
                fgets(nomeBusca, sizeof(nomeBusca), stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = 0; // Remove newline
                BuscaPessoa(nomeBusca);
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
        case 4:
                //Edita cadastro
                EditaCadastro();
                break;
        case 5:
                //Salva lista em arquivo
                SalvaLista("pessoas.dat");
                break;
        case 6:
                LiberaLista();
                //Carrega lista de arquivo
                CarregaLista("pessoas.dat");
                break;
        case 7:
                //Sai do cadastro 
                LiberaLista();
                printf("Saindo do programa...\n");
                break;

        default:
                printf("Opção inválida! Tente novamente.\n");
                break;
    }
    
    if(opcao == 7)
    {
        return;//Retorma para main e finaliza o programa
    }
    else
    {
        system("clear");
        MainMenu();//Chama novamente o menu principal
    }

}
/*---------------Sub-Menu de Cadastro--------------------------------*/
void CadastraMenu(void)
{

    char ch;
    int opcao;
    NODO *pNodo;
    
    system("clear");
    printf("--------- Cadastra Pessoa -----------\n");
    printf("1 - Cadastrar nova pessoa\n");
    printf("2 - Sair do cadastro");
    printf("\n-------------------------------------\n");
    printf("\n");
    printf("Digite sua opção: ");
    scanf("%d", &opcao);
    getchar(); // Limpa o buffer do teclado

    switch(opcao)
    {
        case 1:
                //Cadastra Nova pessoa
                pNodo = NovaPessoa();
                CadastraPessoa(pNodo);
                break;
        case 2:
                //Sai do cadastro 
                return;  //Retorna para MainMenu() 
                //break;

        default:
                printf("Opção inválida! Tente novamente.\n");
                break;
       
    }
    //Volta ao menu de cadastro
    CadastraMenu();
}
/* ------------------------------------------------------ */
void CadastraPessoa(NODO *pNodo)
{
      
    printf("\n-----------------------------------");
    printf("\nDigite o Nome    :");
    fgets(pNodo->pPessoa->nome,sizeof(pNodo->pPessoa->nome), stdin);
    pNodo->pPessoa->nome[strcspn(pNodo->pPessoa->nome, "\n")] = 0;    // Remove o caractere de nova linha, se presente
    printf("Digite a Idade   :");
    scanf("%d", &(pNodo->pPessoa->idade));
    printf("\n-----------------------------------");
        
    return;

    // Função vazia, apenas para evitar erro de compilação
}
/*------------------Estrutura de dados----------------------------------*/
NODO * NovaPessoa(void)
{
    NODO *pNodoAtual = NULL;
    PESSOA *pPessoaAtual = NULL;

    pNodoAtual = (NODO*)malloc(sizeof(NODO));
    pPessoaAtual = (PESSOA*)malloc(sizeof(PESSOA));

    if((pNodoAtual==NULL) || (pPessoaAtual == NULL))
    {
        printf("\nErro: Memória insuficiente\n");
        return(NULL);
    }

    //conecta o novo nodo a nova pessoa
    pNodoAtual->pPessoa = pPessoaAtual;

    if(pInicio == NULL)
    {
        pInicio = pNodoAtual;   // Primeira pessoa
        pFim = pNodoAtual;      // Uma pessoa na lista
        pAux = pNodoAtual;      // Atualiza o auxiliar

        pFim->pAnterior = NULL; //Primeira pessoa não tem vizinhos
        pFim->pProximo = NULL;  //

        return(pFim);

    }
    else
    {
        
        pNodoAtual->pAnterior = pFim;
        pFim->pProximo = pNodoAtual; //Atualiza o fim da lista    
        pFim = pNodoAtual;
        pFim->pProximo = NULL;
                
        return(pFim);               

    }

    //Erro na lógica de inserção na lista
    printf("\nErro: não foi possivel criar nova pessoa.\n");
    return(NULL);

}
/*---------------------------------------------------------------------------------*/
void ListaPessoas(void)
{
    NODO* pAtual = pInicio;

    system("clear");
    printf("--------- Lista de Pessoas Cadastradas -----------\n");
    while(pAtual != NULL)
    {
        printf("\nNome : %s", pAtual->pPessoa->nome);
        printf("\nIdade: %d", pAtual->pPessoa->idade);
        printf("\n-----------------------------------\n");

        pAtual = pAtual->pProximo; // Avança para o próximo nodo
    }   
    printf("Pressione Enter para continuar...");
    getchar(); // Aguarda o usuário pressionar Enter    
}
/*-----------------------------------------------------------------------*/
void SalvaLista(char *nomeArquivo)
{
    FILE *fp;
    NODO* pAtual = pInicio;
    
    fp = fopen(nomeArquivo, "wb");
    if(fp == NULL)
    {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    while(pAtual != NULL)
    {
        fwrite(pAtual->pPessoa, sizeof(PESSOA), 1, fp);
        pAtual = pAtual->pProximo; // Avança para o próximo nodo
    }

    fclose(fp);

}
/*-----------------------------------------------------------------------*/
void CarregaLista(char *nomeArquivo)
{
    FILE *fp;
    NODO *pAux;

    fp = fopen(nomeArquivo, "rb");

    if(fp == NULL)
    {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }
    
    // Aloca e lê diretamente
    pAux = NovaPessoa();
    
    while(pAux != NULL && fread(pAux->pPessoa, sizeof(PESSOA), 1, fp) == 1)
    {
        // Dados já foram lidos para pAux->pPessoa
        // Cria o próximo nodo para a próxima iteração
        pAux = NovaPessoa();
    }
    
    // Remove o último nodo vazio (criado na última iteração)
    if(pAux != NULL)
    {
        if(pAux == pFim)
        {
            if(pAux->pAnterior != NULL)
            {
                pFim = pAux->pAnterior;
                pFim->pProximo = NULL;
            }
            else
            {
                pInicio = NULL;
                pFim = NULL;
            }
        }
        free(pAux->pPessoa);
        free(pAux);
    }

    fclose(fp);
}
/*-----------------------------------------------------------------------*/ 
void LiberaLista(void)
{
    NODO* pAtual = pInicio;
    NODO* pProximo;

    while(pAtual != NULL)
    {
        pProximo = pAtual->pProximo;
        free(pAtual->pPessoa); // Libera a memória da pessoa
        free(pAtual);         // Libera a memória do nodo
        pAtual = pProximo;    // Avança para o próximo nodo
    }

    pInicio = NULL;
    pFim = NULL;
    pAux = NULL;
}
/*-----------------------------------------------------------------------*/
NODO * BuscaPessoa(char *nome)
{
    NODO* pAtual = pInicio;
    
    if(nome == NULL || strlen(nome) == 0)
    {
        printf("Nome inválido para busca.\n");
        return NULL;
    }
    
    while(pAtual != NULL)
    {
        // Busca case-insensitive usando strcasecmp (ou strcmp para case-sensitive)
        if(strcasecmp(pAtual->pPessoa->nome, nome) == 0)
        {
            printf("\n--- Pessoa Encontrada ---\n");
            printf("Nome : %s\n", pAtual->pPessoa->nome);
            printf("Idade: %d\n", pAtual->pPessoa->idade);
            printf("-------------------------\n");
            return pAtual;
        }
        
        pAtual = pAtual->pProximo;
    }
    
    printf("\nPessoa com nome '%s' não encontrada.\n", nome);
    return NULL;
}
/*-----------------------------------------------------------------------*/
void EditaCadastro(void)
{
    char nomeBusca[50];
    char novoNome[50];
    int novaIdade;
    int opcao;
    NODO *pEncontrado;
    
    system("clear");
    printf("--------- Editar Cadastro -----------\n");
    printf("Digite o nome da pessoa a editar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0; // Remove newline
    
    // Busca a pessoa
    pEncontrado = BuscaPessoa(nomeBusca);
    
    if(pEncontrado == NULL)
    {
        printf("\nNão é possível editar. Pessoa não encontrada.\n");
        printf("Pressione Enter para continuar...");
        getchar();
        return;
    }
    
    // Mostra dados atuais
    printf("\n--- Dados Atuais ---\n");
    printf("Nome : %s\n", pEncontrado->pPessoa->nome);
    printf("Idade: %d\n", pEncontrado->pPessoa->idade);
    printf("--------------------\n");
    
    // Menu de edição
    printf("\nO que deseja editar?\n");
    printf("1 - Nome\n");
    printf("2 - Idade\n");
    printf("3 - Ambos\n");
    printf("4 - Cancelar\n");
    printf("Opção: ");
    scanf("%d", &opcao);
    getchar(); // Limpa buffer
    
    switch(opcao)
    {
        case 1:
            // Edita apenas o nome
            printf("\nDigite o novo nome: ");
            fgets(novoNome, sizeof(novoNome), stdin);
            novoNome[strcspn(novoNome, "\n")] = 0;
            strcpy(pEncontrado->pPessoa->nome, novoNome);
            printf("Nome atualizado com sucesso!\n");
            break;
            
        case 2:
            // Edita apenas a idade
            printf("\nDigite a nova idade: ");
            scanf("%d", &novaIdade);
            getchar(); // Limpa buffer
            pEncontrado->pPessoa->idade = novaIdade;
            printf("Idade atualizada com sucesso!\n");
            break;
            
        case 3:
            // Edita nome e idade
            printf("\nDigite o novo nome: ");
            fgets(novoNome, sizeof(novoNome), stdin);
            novoNome[strcspn(novoNome, "\n")] = 0;
            strcpy(pEncontrado->pPessoa->nome, novoNome);
            
            printf("Digite a nova idade: ");
            scanf("%d", &novaIdade);
            getchar(); // Limpa buffer
            pEncontrado->pPessoa->idade = novaIdade;
            
            printf("Dados atualizados com sucesso!\n");
            break;
            
        case 4:
            printf("Edição cancelada.\n");
            break;
            
        default:
            printf("Opção inválida!\n");
            break;
    }
    
    // Mostra dados após edição (se editou)
    if(opcao >= 1 && opcao <= 3)
    {
        printf("\n--- Dados Atualizados ---\n");
        printf("Nome : %s\n", pEncontrado->pPessoa->nome);
        printf("Idade: %d\n", pEncontrado->pPessoa->idade);
        printf("-------------------------\n");
    }
    
    printf("\nPressione Enter para continuar...");
    getchar();
}
/*-----------------------------------------------------------------------*/
// Fim do código