#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bd_times.h"
#include "bd_partidas.h"
#include "time.h"

void consultar_time(BDTimes* bd_times) {
    char prefixo[50];
    printf("Digite o nome ou prefixo do time: ");
    scanf("%s", prefixo);
    
    Time* resultados[10];
    int num_encontrados = bd_times_buscar_por_prefixo(bd_times, prefixo, resultados);
    
    if (num_encontrados == 0) {
        printf("Nenhum time encontrado com o prefixo '%s'.\n", prefixo);
    } else {
        printf("ID Time        V E D GM GS S  PG\n");
        for (int i = 0; i < num_encontrados; i++) {
            Time* t = resultados[i];
            printf("%-2d %-11s %d %d %d %-2d %-2d %-3d %-2d\n", 
                   t->id, t->nome, t->vitorias, t->empates, t->derrotas,
                   t->gols_marcados, t->gols_sofridos, 
                   time_calcular_saldo(t), time_calcular_pontos(t));
        }
    }
}

void consultar_partidas(BDTimes* bd_times, BDPartidas* bd_partidas) {
    int modo;
    printf("Escolha o modo de consulta:\n");
    printf("1 - Por time mandante\n");
    printf("2 - Por time visitante\n");
    printf("3 - Por time mandante ou visitante\n");
    printf("4 - Retornar ao menu principal\n");
    printf("Opcao: ");
    scanf("%d", &modo);
    
    if (modo == 4) return;
    
    char nome[50];
    printf("Digite o nome: ");
    scanf("%s", nome);
    
    Time* times_encontrados[10];
    int num_times = bd_times_buscar_por_prefixo(bd_times, nome, times_encontrados);
    
    if (num_times == 0) {
        printf("Nenhum time encontrado com o nome '%s'.\n", nome);
        return;
    }
    
    printf("ID Time1      x Time2      Placar\n");
    int partidas_encontradas = 0;
    
    for (int i = 0; i < num_times; i++) {
        Time* time = times_encontrados[i];
        Partida* resultados[100];
        int num_partidas = bd_partidas_buscar_por_time(bd_partidas, time->id, modo, resultados);
        
        for (int j = 0; j < num_partidas; j++) {
            Partida* p = resultados[j];
            Time* time1 = bd_times_buscar_por_id(bd_times, p->time1_id);
            Time* time2 = bd_times_buscar_por_id(bd_times, p->time2_id);
            
            if (time1 && time2) {
                printf("%-2d %-10s x %-10s %d x %d\n", 
                       p->id, time1->nome, time2->nome, p->gols_time1, p->gols_time2);
                partidas_encontradas++;
            }
        }
    }
    
    if (partidas_encontradas == 0) {
        printf("Nenhuma partida encontrada.\n");
    }
}

void inserir_partida(BDTimes* bd_times, BDPartidas* bd_partidas) {
    int time1_id, time2_id, gols1, gols2;
    printf("Para inserir um novo registro, digite os valores para\n");
    printf("os campos Time1, Time2, Placar1 e Placar2:\n");
    
    printf("Time1 (ID): ");
    scanf("%d", &time1_id);
    printf("Time2 (ID): ");
    scanf("%d", &time2_id);
    printf("Placar1: ");
    scanf("%d", &gols1);
    printf("Placar2: ");
    scanf("%d", &gols2);
    
    Time* time1 = bd_times_buscar_por_id(bd_times, time1_id);
    Time* time2 = bd_times_buscar_por_id(bd_times, time2_id);
    
    if (time1 == NULL || time2 == NULL) {
        printf("Erro: Time1 ou Time2 não encontrado.\n");
        return;
    }
    
    Partida partida;
    partida.time1_id = time1_id;
    partida.time2_id = time2_id;
    partida.gols_time1 = gols1;
    partida.gols_time2 = gols2;
    
    printf("Confirma a insercao do registro abaixo? (S/N)\n");
    printf("ID Time1      Time2      Placar1 Placar2\n");
    printf("?  %-10s %-10s %-7d %-7d\n", time1->nome, time2->nome, gols1, gols2);
    
    char confirmacao;
    scanf(" %c", &confirmacao);
    if (confirmacao == 'S' || confirmacao == 's') {
        bd_partidas_inserir(bd_partidas, partida);
        bd_times_calcular_estatisticas(bd_times, bd_partidas);
        printf("Registro inserido com sucesso.\n");
    } else {
        printf("Insercao cancelada.\n");
    }
}

void atualizar_partida(BDTimes* bd_times, BDPartidas* bd_partidas) {
    printf("Para atualizar, primeiro consulte as partidas para obter o ID.\n");
    consultar_partidas(bd_times, bd_partidas);
    
    int id;
    printf("Digite o ID do registro a ser atualizado: ");
    scanf("%d", &id);
    
    Partida* partida = bd_partidas_buscar_por_id(bd_partidas, id);
    if (partida == NULL) {
        printf("Partida com ID %d não encontrada.\n", id);
        return;
    }
    
    Time* time1 = bd_times_buscar_por_id(bd_times, partida->time1_id);
    Time* time2 = bd_times_buscar_por_id(bd_times, partida->time2_id);
    
    printf("Digite o novo valor para os campos Placar1 e Placar2\n");
    printf("(para manter o valor atual de um campo, digite '-'):\n");
    
    char entrada1[10], entrada2[10];
    printf("Placar1: ");
    scanf("%s", entrada1);
    printf("Placar2: ");
    scanf("%s", entrada2);
    
    int novo_gols1 = partida->gols_time1;
    int novo_gols2 = partida->gols_time2;
    
    if (strcmp(entrada1, "-") != 0) {
        novo_gols1 = atoi(entrada1);
    }
    if (strcmp(entrada2, "-") != 0) {
        novo_gols2 = atoi(entrada2);
    }
    
    printf("Confirma os novos valores para o registro abaixo? (S/N)\n");
    printf("ID Time1      Time2      Placar1 Placar2\n");
    printf("%-2d %-10s %-10s %-7d %-7d\n", 
           partida->id, time1->nome, time2->nome, novo_gols1, novo_gols2);
    
    char confirmacao;
    scanf(" %c", &confirmacao);
    if (confirmacao == 'S' || confirmacao == 's') {
        partida->gols_time1 = novo_gols1;
        partida->gols_time2 = novo_gols2;
        bd_times_calcular_estatisticas(bd_times, bd_partidas);
        printf("Registro atualizado com sucesso.\n");
    } else {
        printf("Atualizacao cancelada.\n");
    }
}

void remover_partida(BDTimes* bd_times, BDPartidas* bd_partidas) {
    printf("Para remover, primeiro consulte as partidas para obter o ID.\n");
    consultar_partidas(bd_times, bd_partidas);
    
    int id;
    printf("Digite o ID do registro a ser removido: ");
    scanf("%d", &id);
    
    Partida* partida = bd_partidas_buscar_por_id(bd_partidas, id);
    if (partida == NULL) {
        printf("Partida com ID %d não encontrada.\n", id);
        return;
    }
    
    Time* time1 = bd_times_buscar_por_id(bd_times, partida->time1_id);
    Time* time2 = bd_times_buscar_por_id(bd_times, partida->time2_id);
    
    printf("Tem certeza de que deseja excluir o registro abaixo? (S/N)\n");
    printf("ID Time1      Time2      Placar1 Placar2\n");
    printf("%-2d %-10s %-10s %-7d %-7d\n", 
           partida->id, time1->nome, time2->nome, partida->gols_time1, partida->gols_time2);
    
    char confirmacao;
    scanf(" %c", &confirmacao);
    if (confirmacao == 'S' || confirmacao == 's') {
        bd_partidas_remover(bd_partidas, id);
        bd_times_calcular_estatisticas(bd_times, bd_partidas);
        printf("Registro removido com sucesso.\n");
    } else {
        printf("Remocao cancelada.\n");
    }
}

int main() {
    BDTimes* bd_times = bd_times_carregar("times.csv");
    if (!bd_times) {
        printf("Erro ao carregar times.\n");
        return 1;
    }
    
    BDPartidas* bd_partidas = bd_partidas_carregar("partidas.csv");
    if (!bd_partidas) {
        printf("Erro ao carregar partidas.\n");
        bd_times_liberar(bd_times);
        return 1;
    }
    
    bd_times_calcular_estatisticas(bd_times, bd_partidas);
    
    char opcao;
    do {
        printf("\n=== Campeonato Computacional de Futebol ===\n");
        printf("1 - Consultar time\n");
        printf("2 - Consultar partidas\n");
        printf("3 - Atualizar partida\n");
        printf("4 - Remover partida\n");
        printf("5 - Inserir partida\n");
        printf("6 - Imprimir tabela\n");
        printf("Q - Sair\n");
        printf("Opcao: ");
        scanf(" %c", &opcao);
        
        switch(opcao) {
            case '1': 
                consultar_time(bd_times); 
                break;
            case '2': 
                consultar_partidas(bd_times, bd_partidas); 
                break;
            case '3': 
                atualizar_partida(bd_times, bd_partidas); 
                break;
            case '4': 
                remover_partida(bd_times, bd_partidas); 
                break;
            case '5': 
                inserir_partida(bd_times, bd_partidas); 
                break;
            case '6': 
                bd_times_imprimir_tabela_ordenada(bd_times); 
                break;
            case 'Q':
            case 'q':
                printf("Encerrando sistema...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while(opcao != 'Q' && opcao != 'q');
    
    bd_times_liberar(bd_times);
    bd_partidas_liberar(bd_partidas);
    
    return 0;
}