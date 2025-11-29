#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- CONSTANTES DO JOGO ---
#define CAPACIDADE_FILA 5       // Capacidade fixa da fila circular de peças futuras.
#define CAPACIDADE_PILHA 3      // Capacidade máxima da pilha de peças reservadas.
#define TIPOS_PECAS "IOTL"      // Tipos de peças disponíveis para geração.

// --- ESTRUTURAS DE DADOS ---

// Estrutura para armazenar as informações de uma única peça.
struct Peca {
    char nome;     // Tipo da peça ('I', 'O', 'T', 'L')
    int id;        // Identificador numérico único
};

// Estrutura para gerenciar a FILA CIRCULAR de peças futuras (FIFO).
struct FilaPecas {
    struct Peca elementos[CAPACIDADE_FILA]; // Array que armazena as peças
    int frente;                             // Índice da peça na frente (DEQUEUE)
    int tras;                               // Índice da peça no final (ENQUEUE)
    int contador;                           // Número atual de elementos na fila
};

// Estrutura para gerenciar a PILHA de peças reservadas (LIFO).
struct PilhaReserva {
    struct Peca elementos[CAPACIDADE_PILHA]; // Array que armazena as peças
    int topo;                                // Índice do topo da pilha (LIFO)
};

// Variável global para garantir que cada nova peça tenha um ID exclusivo.
int proximo_id_peca = 0;


// --- PROTÓTIPOS DAS FUNÇÕES ---

// Geração de Peça
struct Peca gerarPeca();

// Funções de Gerenciamento da Fila Circular (FIFO)
void inicializarFila(struct FilaPecas *fila);
void inserirFila(struct FilaPecas *fila, struct Peca novaPeca); // Enqueue
struct Peca removerFila(struct FilaPecas *fila);               // Dequeue
void exibirFila(struct FilaPecas *fila);
int filaVazia(struct FilaPecas *fila);
int filaCheia(struct FilaPecas *fila);

// Funções de Gerenciamento da Pilha (LIFO)
void inicializarPilha(struct PilhaReserva *pilha);
void pushPilha(struct PilhaReserva *pilha, struct Peca peca);   // Push (Reservar)
struct Peca popPilha(struct PilhaReserva *pilha);               // Pop (Usar)
void exibirPilha(struct PilhaReserva *pilha);
int pilhaVazia(struct PilhaReserva *pilha);
int pilhaCheia(struct PilhaReserva *pilha);

// Funções de Ação do Jogo
void jogarPeca(struct FilaPecas *fila);
void reservarPeca(struct FilaPecas *fila, struct PilhaReserva *pilha);
void usarPecaReservada(struct PilhaReserva *pilha);
void trocarPecaAtual(struct FilaPecas *fila, struct PilhaReserva *pilha);
void trocaMultipla(struct FilaPecas *fila, struct PilhaReserva *pilha);


// --- INÍCIO DA FUNÇÃO PRINCIPAL ---

int main() {
    
    // Declaração das estruturas principais.
    struct FilaPecas filaDeEspera;
    struct PilhaReserva pilhaDeReserva;
    int opcao;
    
    srand(time(NULL)); 
    
    inicializarFila(&filaDeEspera);
    inicializarPilha(&pilhaDeReserva);

    printf("=======================================================\n");
    printf("        GERENCIADOR DE PEÇAS TETRIS STACK (AVANÇADO)   \n");
    printf("=======================================================\n\n");

    printf("INICIALIZANDO A FILA DE PEÇAS FUTURAS COM %d ELEMENTOS...\n", CAPACIDADE_FILA);
    
    // Preenche a fila inicial até a capacidade máxima.
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        inserirFila(&filaDeEspera, gerarPeca());
    }
    
    // Exibe o estado inicial do jogo.
    exibirFila(&filaDeEspera);
    exibirPilha(&pilhaDeReserva);
    
    // Laço principal do jogo (Menu de interação).
    do {
        printf("-------------------------------------------------------\n");
        printf("Opções disponíveis:\n");
        printf("Código | Ação\n");
        printf("-------------------------------------------------------\n");
        printf("  1    | Jogar peça da frente da fila\n");
        printf("  2    | Enviar peça da fila para a pilha de reserva\n");
        printf("  3    | Usar peça da pilha de reserva\n");
        printf("  4    | Trocar peça da frente da fila com o topo da pilha\n");
        printf("  5    | Trocar os %d primeiros da fila com as %d peças da pilha\n", CAPACIDADE_PILHA, CAPACIDADE_PILHA);
        printf("  0    | Sair\n");
        printf("-------------------------------------------------------\n");
        printf("Opção escolhida: ");
        
        scanf("%d", &opcao);
        while (getchar() != '\n'); 

        printf("\n");

        // Estrutura Condicional (Switch-Case) para tratamento das ações.
        switch (opcao) {
            case 1:
                jogarPeca(&filaDeEspera);
                break;
            case 2:
                reservarPeca(&filaDeEspera, &pilhaDeReserva);
                break;
            case 3:
                usarPecaReservada(&pilhaDeReserva);
                break;
            case 4:
                trocarPecaAtual(&filaDeEspera, &pilhaDeReserva);
                break;
            case 5:
                trocaMultipla(&filaDeEspera, &pilhaDeReserva);
                break;
            case 0:
                printf("SAINDO... Finalizando o Gerenciador de Peças Tetris Stack!\n");
                break;
            default:
                printf("⚠️ OPÇÃO INVÁLIDA! Por favor, escolha uma opção do menu.\n\n");
                break;
        }
        
        // Exibe o estado após qualquer ação, exceto Sair.
        if (opcao != 0) {
             printf("NOVO ESTADO:\n");
             exibirFila(&filaDeEspera);
             exibirPilha(&pilhaDeReserva);
        }

    } while (opcao != 0);

    return 0;
}


// --- IMPLEMENTAÇÃO DAS FUNÇÕES DE GERAÇÃO E ESTRUTURAS ---

/**
 * @brief Gera uma nova peça com tipo aleatório e ID único.
 */
struct Peca gerarPeca() {
    struct Peca novaPeca;
    int indice_aleatorio;
    
    indice_aleatorio = rand() % strlen(TIPOS_PECAS);
    novaPeca.nome = TIPOS_PECAS[indice_aleatorio];
    
    novaPeca.id = proximo_id_peca++; 
    
    return novaPeca;
}

// --- FUNÇÕES DE FILA CIRCULAR (FIFO) ---

void inicializarFila(struct FilaPecas *fila) {
    fila->frente = 0; 
    fila->tras = CAPACIDADE_FILA - 1; 
    fila->contador = 0;
}

int filaVazia(struct FilaPecas *fila) {
    return (fila->contador == 0);
}

int filaCheia(struct FilaPecas *fila) {
    return (fila->contador == CAPACIDADE_FILA);
}

/**
 * @brief Adiciona uma nova peça ao final da Fila (Enqueue).
 */
void inserirFila(struct FilaPecas *fila, struct Peca novaPeca) {
    if (filaCheia(fila)) {
        return; // A fila não deve estar cheia se esta função for chamada no loop principal.
    }
    
    fila->tras = (fila->tras + 1) % CAPACIDADE_FILA;
    fila->elementos[fila->tras] = novaPeca; 
    fila->contador++;
}

/**
 * @brief Remove a peça da frente da Fila (Dequeue).
 */
struct Peca removerFila(struct FilaPecas *fila) {
    struct Peca pecaRemovida = fila->elementos[fila->frente];
    
    fila->frente = (fila->frente + 1) % CAPACIDADE_FILA;
    fila->contador--;
    
    return pecaRemovida;
}

/**
 * @brief Exibe o estado atual da Fila.
 */
void exibirFila(struct FilaPecas *fila) {
    printf("Fila de peças (%d/%d): ", fila->contador, CAPACIDADE_FILA);
    
    if (filaVazia(fila)) {
        printf("[VAZIA]\n");
        return;
    }
    
    int i;
    int indice_atual = fila->frente;
    
    for (i = 0; i < fila->contador; i++) {
        printf("[%c %d]", fila->elementos[indice_atual].nome, fila->elementos[indice_atual].id);
        
        if (i < fila->contador - 1) {
            printf(" -> ");
        }
        
        indice_atual = (indice_atual + 1) % CAPACIDADE_FILA;
    }
    
    printf("\n");
}

// --- FUNÇÕES DE PILHA LINEAR (LIFO) ---

void inicializarPilha(struct PilhaReserva *pilha) {
    pilha->topo = -1; 
}

int pilhaVazia(struct PilhaReserva *pilha) {
    return (pilha->topo == -1);
}

int pilhaCheia(struct PilhaReserva *pilha) {
    return (pilha->topo == CAPACIDADE_PILHA - 1);
}

/**
 * @brief Adiciona uma peça ao topo da Pilha (Push).
 */
void pushPilha(struct PilhaReserva *pilha, struct Peca peca) {
    pilha->topo++;
    pilha->elementos[pilha->topo] = peca;
}

/**
 * @brief Remove e retorna a peça do topo da Pilha (Pop).
 */
struct Peca popPilha(struct PilhaReserva *pilha) {
    struct Peca pecaRemovida = pilha->elementos[pilha->topo];
    pilha->topo--;
    return pecaRemovida;
}

/**
 * @brief Exibe o estado atual da Pilha de Reserva.
 */
void exibirPilha(struct PilhaReserva *pilha) {
    printf("Pilha de reserva (Topo -> Base) (%d/%d): ", pilha->topo + 1, CAPACIDADE_PILHA);
    
    if (pilhaVazia(pilha)) {
        printf("[VAZIA]\n\n");
        return;
    }
    
    for (int i = pilha->topo; i >= 0; i--) {
        printf("[%c %d]", pilha->elementos[i].nome, pilha->elementos[i].id);
        if (i > 0) {
            printf(" -> ");
        }
    }
    printf("\n\n");
}


// --- FUNÇÕES DE AÇÃO DO JOGO (COM TROCA DE PEÇAS) ---

/**
 * @brief Simula o ato de 'Jogar' a peça da frente da Fila.
 * Adiciona uma nova peça à fila para manter o tamanho (Regra do Jogo).
 */
void jogarPeca(struct FilaPecas *fila) {
    if (filaVazia(fila)) {
        printf("⚠️ ERRO: A fila está vazia! Nenhuma peça para jogar.\n");
        return;
    }
    
    struct Peca pecaJogada = removerFila(fila);
    printf("Ação: Peça JOGADA (removida da frente): [%c %d].\n", pecaJogada.nome, pecaJogada.id);
    
    // Regra: Uma nova peça é gerada para manter a fila cheia.
    struct Peca novaPeca = gerarPeca();
    inserirFila(fila, novaPeca);
    printf("Peça gerada: [%c %d] inserida no final da fila.\n\n", novaPeca.nome, novaPeca.id);
}

/**
 * @brief Move a peça da frente da Fila para o Topo da Pilha (Reservar).
 * Adiciona uma nova peça à fila para manter o tamanho (Regra do Jogo).
 */
void reservarPeca(struct FilaPecas *fila, struct PilhaReserva *pilha) {
    if (filaVazia(fila)) {
        printf("⚠️ ERRO: A fila está vazia! Não há peça para reservar.\n");
        return;
    }
    if (pilhaCheia(pilha)) {
        printf("⚠️ ERRO: A pilha de reserva está cheia! Não é possível reservar mais peças.\n");
        return;
    }
    
    struct Peca pecaReservada = removerFila(fila);
    pushPilha(pilha, pecaReservada);
    printf("Ação: Peça [%c %d] movida da Fila para o Topo da Pilha (RESERVADA).\n", pecaReservada.nome, pecaReservada.id);
    
    // Regra: Uma nova peça é gerada para manter a fila cheia.
    struct Peca novaPeca = gerarPeca();
    inserirFila(fila, novaPeca);
    printf("Peça gerada: [%c %d] inserida no final da fila.\n\n", novaPeca.nome, novaPeca.id);
}

/**
 * @brief Simula o ato de 'Usar' uma peça reservada (remove do Topo da Pilha).
 * Note que esta ação NÃO remove nada da fila, então a fila permanece cheia.
 */
void usarPecaReservada(struct PilhaReserva *pilha) {
    if (pilhaVazia(pilha)) {
        printf("⚠️ ERRO: A pilha de reserva está vazia! Nenhuma peça para usar.\n");
        return;
    }
    
    struct Peca pecaUsada = popPilha(pilha);
    printf("Ação: Peça [%c %d] usada (removida do Topo da Pilha).\n\n", pecaUsada.nome, pecaUsada.id);
    
    // NOTA: A regra de gerar nova peça se aplica a ações que reduzem o tamanho da fila (Jogar ou Reservar).
    // Usar a reserva não afeta a fila.
}

/**
 * @brief Troca a peça da Fila (frente) com a peça do Topo da Pilha.
 * Esta ação não afeta o contador de elementos em nenhuma estrutura.
 */
void trocarPecaAtual(struct FilaPecas *fila, struct PilhaReserva *pilha) {
    if (filaVazia(fila)) {
        printf("⚠️ ERRO: A fila está vazia! Não há peça para trocar.\n");
        return;
    }
    if (pilhaVazia(pilha)) {
        printf("⚠️ ERRO: A pilha está vazia! Não há peça para trocar.\n");
        return;
    }
    
    // Estrutura temporária para realizar a troca sem perda de dados.
    struct Peca temp;
    
    // A peça da frente da Fila
    struct Peca *pecaFila = &fila->elementos[fila->frente];
    // A peça do Topo da Pilha
    struct Peca *pecaPilha = &pilha->elementos[pilha->topo];

    // Troca de valores:
    temp = *pecaFila;      // 1. Guarda a peça da fila
    *pecaFila = *pecaPilha; // 2. Coloca a peça da pilha na fila
    *pecaPilha = temp;      // 3. Coloca a peça guardada (da fila) na pilha
    
    printf("Ação: TROCA UNITÁRIA realizada. A peça [%c %d] da fila e a peça [%c %d] da pilha foram trocadas.\n\n", 
           pecaPilha->nome, pecaPilha->id, pecaFila->nome, pecaFila->id);
}

/**
 * @brief Troca as três primeiras peças da Fila com as três peças da Pilha.
 * A Pilha deve estar cheia (3) e a Fila deve ter, pelo menos, 3 elementos.
 */
void trocaMultipla(struct FilaPecas *fila, struct PilhaReserva *pilha) {
    // A pilha só pode ter 3 peças, então checamos se está cheia.
    if (!pilhaCheia(pilha)) {
        printf("⚠️ ERRO: A pilha de reserva deve estar CHEIA (%d peças) para a troca em bloco.\n", CAPACIDADE_PILHA);
        return;
    }
    
    // Checamos se a fila tem, pelo menos, 3 elementos.
    if (fila->contador < CAPACIDADE_PILHA) {
        printf("⚠️ ERRO: A fila deve ter, pelo menos, %d peças para a troca em bloco (Atualmente: %d).\n", 
               CAPACIDADE_PILHA, fila->contador);
        return;
    }
    
    // Realiza a troca das 3 peças.
    // O loop percorre a pilha de 0 a 2 (base ao topo) e a fila a partir da frente (circularmente).
    printf("Ação: Início da TROCA MÚLTIPLA em Bloco (%d peças).\n", CAPACIDADE_PILHA);
    
    struct Peca temp;
    int indice_fila_atual = fila->frente;
    
    for (int i = 0; i < CAPACIDADE_PILHA; i++) {
        // As peças da pilha são acessadas diretamente pelo índice (0, 1, 2)
        struct Peca *pecaPilha = &pilha->elementos[i];
        
        // As peças da fila são acessadas pela lógica circular
        struct Peca *pecaFila = &fila->elementos[indice_fila_atual];
        
        // Troca de valores:
        temp = *pecaFila;
        *pecaFila = *pecaPilha;
        *pecaPilha = temp;

        // Move para o próximo índice na fila circular
        indice_fila_atual = (indice_fila_atual + 1) % CAPACIDADE_FILA;
    }
    
    printf("Ação: TROCA MÚLTIPLA realizada entre os %d primeiros da Fila e as %d peças da Pilha.\n\n", 
           CAPACIDADE_PILHA, CAPACIDADE_PILHA);
}