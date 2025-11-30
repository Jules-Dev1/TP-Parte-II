#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include "bd_times.h"
#include "bd_partidas.h"

BDTimes* bd_times_carregar(const char* filename) {
    FILE* arquivo = fopen(filename, "r");
    if (!arquivo) {
        printf("Erro ao abrir arquivo %s\n", filename);
        return NULL;
    }
    
    BDTimes* bd = (BDTimes*)malloc(sizeof(BDTimes));
    if (!bd) {
        fclose(arquivo);
        return NULL;
    }
    
    bd->primeiro = NULL;
    bd->num_times = 0;
    
    char linha[256];
    fgets(linha, sizeof(linha), arquivo);
    
    while (fgets(linha, sizeof(linha), arquivo)) {
        Time time;
        linha[strcspn(linha, "\n")] = 0;
        
        char* token = strtok(linha, ",");
        if (token) time.id = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(time.nome, token);
        
        time.vitorias = 0;
        time.empates = 0;
        time.derrotas = 0;
        time.gols_marcados = 0;
        time.gols_sofridos = 0;
        
        NoTime* novo_no = (NoTime*)malloc(sizeof(NoTime));
        novo_no->time = time;
        novo_no->prox = bd->primeiro;
        bd->primeiro = novo_no;
        bd->num_times++;
    }
    
    fclose(arquivo);
    return bd;
}

void bd_times_liberar(BDTimes* bd) {
    NoTime* no = bd->primeiro;
    while (no != NULL) {
        NoTime* proximo = no->prox;
        free(no);
        no = proximo;
    }
    free(bd);
}

Time* bd_times_buscar_por_id(BDTimes* bd, int id) {
    NoTime* no = bd->primeiro;
    while (no != NULL) {
        if (no->time.id == id) {
            return &no->time;
        }
        no = no->prox;
    }
    return NULL;
}

int bd_times_buscar_por_prefixo(BDTimes* bd, const char* prefixo, Time* resultados[]) {
    int count = 0;
    int len_prefixo = strlen(prefixo);
    NoTime* no = bd->primeiro;
    
    while (no != NULL && count < 10) {
        if (strncasecmp(no->time.nome, prefixo, len_prefixo) == 0) {
            resultados[count++] = &no->time;
        }
        no = no->prox;
    }
    
    return count;
}

void bd_times_calcular_estatisticas(BDTimes* bd_times, void* bd_partidas) {
    BDPartidas* partidas = (BDPartidas*)bd_partidas;
    
    // Reiniciar estatísticas de todos os times
    NoTime* no_time = bd_times->primeiro;
    while (no_time != NULL) {
        no_time->time.vitorias = 0;
        no_time->time.empates = 0;
        no_time->time.derrotas = 0;
        no_time->time.gols_marcados = 0;
        no_time->time.gols_sofridos = 0;
        no_time = no_time->prox;
    }
    
    // Calcular estatísticas baseadas nas partidas
    NoPartida* no_partida = partidas->primeiro;
    while (no_partida != NULL) {
        Partida* p = &no_partida->partida;
        Time* time1 = bd_times_buscar_por_id(bd_times, p->time1_id);
        Time* time2 = bd_times_buscar_por_id(bd_times, p->time2_id);
        
        if (time1 && time2) {
            time_atualizar_estatisticas(time1, p->gols_time1, p->gols_time2);
            time_atualizar_estatisticas(time2, p->gols_time2, p->gols_time1);
        }
        no_partida = no_partida->prox;
    }
}

static int comparar_times(const void* a, const void* b) {
    Time* time1 = *(Time**)a;
    Time* time2 = *(Time**)b;

    int pg1 = time_calcular_pontos(time1);
    int pg2 = time_calcular_pontos(time2);

    if (pg1 != pg2) {
        return pg2 - pg1;
    }

    if (time1->vitorias != time2->vitorias) {
        return time2->vitorias - time1->vitorias;
    }

    int saldo1 = time_calcular_saldo(time1);
    int saldo2 = time_calcular_saldo(time2);
    return saldo2 - saldo1;
}

void bd_times_imprimir_tabela_ordenada(BDTimes* bd_times) {
    Time** array_times = (Time**)malloc(bd_times->num_times * sizeof(Time*));
    NoTime* no = bd_times->primeiro;
    int i = 0;
    
    while (no != NULL) {
        array_times[i++] = &no->time;
        no = no->prox;
    }

    qsort(array_times, bd_times->num_times, sizeof(Time*), comparar_times);

    printf("ID Time        V E D GM GS S  PG\n");
    for (i = 0; i < bd_times->num_times; i++) {
        Time* t = array_times[i];
        printf("%-2d %-11s %d %d %d %-2d %-2d %-3d %-2d\n", 
               t->id, t->nome, t->vitorias, t->empates, t->derrotas,
               t->gols_marcados, t->gols_sofridos, 
               time_calcular_saldo(t), time_calcular_pontos(t));
    }

    free(array_times);
}