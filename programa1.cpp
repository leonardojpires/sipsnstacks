#include <cstdio>
#include <cstring>
#include <iostream>
#include <unistd.h>

using namespace std;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

struct Produto {
    int prateleira;
    int posicao;
    char nome[50];
    char tipo; // C, A, S, R, L
    char marca[50];
    double preco;
    char validade[20]; // Opcional
    int quantidade;
};

const int MAX_QUANTIDADE = 10;
const int MAX_PRODUTOS = 100; // M�ximo de produtos armazenados

// Vetor para armazenar os produtos
Produto produtos[MAX_PRODUTOS];
int numProdutos = 0;  // Contador de produtos

double dinheiroTotal = 0.0;

// Fun��es para manipular o arquivo produtos.txt
void salvarProdutoArquivo() {
    FILE* arquivo = fopen("produtos.txt", "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    for (int i = 0; i < numProdutos; ++i) {
        Produto& produto = produtos[i];
        fprintf(arquivo, "%d %d %s %c %s %.2f %s %d\n",
                produto.prateleira, produto.posicao, produto.nome, produto.tipo, produto.marca, produto.preco,
                strlen(produto.validade) > 0 ? produto.validade : "N/A", produto.quantidade);
    }

    fclose(arquivo);
    printf("Produtos salvos no arquivo com sucesso.\n");
}

void carregarProdutoArquivo() {
    FILE* arquivo = fopen("produtos.txt", "r");
    if (!arquivo) {
        printf("Arquivo produtos.txt n�o encontrado, iniciando sem dados.\n");
        return;
    }

    Produto produto;
    while (fscanf(arquivo, "%d %d %49s %c %49s %lf %19s %d",
                  &produto.prateleira, &produto.posicao, produto.nome, &produto.tipo, produto.marca,
                  &produto.preco, produto.validade, &produto.quantidade) != EOF) {
        if (numProdutos < MAX_PRODUTOS) {
            produtos[numProdutos++] = produto;
        }
    }

    fclose(arquivo);
    printf("Produtos carregados do arquivo com sucesso.\n");
}

void removerProduto(int prateleira, int posicao) {
    int encontrado = 0;

    // Encontrar e remover o produto
    for (int i = 0; i < numProdutos; ++i) {
        Produto& produto = produtos[i];
        if (produto.prateleira == prateleira && produto.posicao == posicao) {
            // Move os elementos para a esquerda
            for (int j = i; j < numProdutos - 1; ++j) {
                produtos[j] = produtos[j + 1];
            }
            numProdutos--; // Diminui o n�mero de produtos
            printf("Produto removido com sucesso.\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Produto n�o encontrado na posi��o indicada.\n");
    }
}

void inserirProduto(int prateleira, int posicao) {
    if (numProdutos >= MAX_PRODUTOS) {
        printf("M�ximo de produtos alcan�ado.\n");
        return;
    }

    Produto& produto = produtos[numProdutos];

    produto.prateleira = prateleira;
    produto.posicao = posicao;

    printf("Nome do produto: ");
    scanf(" %[^\n]", produto.nome);

    printf("Tipo do produto (C - cerveja; A - �gua; S - sumo; R - refrigerante; L - leite): ");
    scanf(" %c", &produto.tipo);

    printf("Marca: ");
    scanf(" %[^\n]", produto.marca);

    printf("Pre�o: ");
    scanf("%lf", &produto.preco);

    printf("Data de validade (opcional, pressione Enter para pular): ");
    scanf(" %[^\n]", produto.validade);

    printf("Quantidade (m�ximo %d): ", MAX_QUANTIDADE);
    scanf("%d", &produto.quantidade);

    if (produto.quantidade > MAX_QUANTIDADE) {
        produto.quantidade = MAX_QUANTIDADE;
        printf("Quantidade ajustada para o m�ximo permitido.\n");
    }

    numProdutos++; // Incrementa o n�mero de produtos
    printf("Produto inserido com sucesso na prateleira %d, posi��o %d.\n", prateleira, posicao);
}

void listarProdutos() {
    printf("\nProdutos dispon�veis:\n");
    for (int i = 0; i < numProdutos; ++i) {
        Produto& produto = produtos[i];
        printf("Prateleira %d Posi��o %d: %s (%c) - Marca: %s - Pre�o: %.2f� - Quantidade: %d - Validade: %s\n",
               produto.prateleira, produto.posicao, produto.nome, produto.tipo, produto.marca, produto.preco,
               produto.quantidade, strlen(produto.validade) > 0 ? produto.validade : "N/A");
    }
}

void mostrarDinheiroAcumulado() {
    printf("Dinheiro acumulado na m�quina: %.2f�.\n", dinheiroTotal);
}

int main() {
    carregarProdutoArquivo(); // Carregar produtos do arquivo na inicializa��o

    int opcao;
    do {
        printf("\n--- M�quina de Vending ---\n");
        printf("1. Inserir produto\n");
        printf("2. Listar todos os produtos\n");
        printf("3. Remover produto\n");
        printf("4. Mostrar dinheiro acumulado\n");
        printf("0. Sair\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao);

        int prateleira, posicao;

        switch (opcao) {
            case 1:
                clearScreen();
                printf("N�mero da prateleira (0 a 7): ");
                scanf("%d", &prateleira);
                printf("N�mero da posi��o (0 a 7): ");
                scanf("%d", &posicao);
                inserirProduto(prateleira, posicao);
                salvarProdutoArquivo();
                sleep(3);
                clearScreen(); // Salvar ap�s inserir
                break;
            case 2:
                listarProdutos();
                break;
            case 3:
                printf("N�mero da prateleira (0 a 7): ");
                scanf("%d", &prateleira);
                printf("N�mero da posi��o (0 a 7): ");
                scanf("%d", &posicao);
                removerProduto(prateleira, posicao);
                salvarProdutoArquivo(); // Salvar ap�s remover
                sleep(3);
                clearScreen();
                break;
            case 4:
                mostrarDinheiroAcumulado();
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Op��o inv�lida!\n");
        }
    } while (opcao != 0);

    salvarProdutoArquivo(); // Salvar antes de sair
    return 0;
}
