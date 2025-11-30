#ifndef BD_TIMES_H
#define BD_TIMES_H

#include "time.h"

typedef struct no_time {
    Time time;
    struct no_time* prox;
} NoTime;

typedef struct {
    NoTime* primeiro;
    int num_times;
} BDTimes;

BDTimes* bd_times_carregar(const char* filename);
void bd_times_liberar(BDTimes* bd);
Time* bd_times_buscar_por_id(BDTimes* bd, int id);
int bd_times_buscar_por_prefixo(BDTimes* bd, const char* prefixo, Time* resultados[]);
void bd_times_calcular_estatisticas(BDTimes* bd_times, void* bd_partidas);
void bd_times_imprimir_tabela_ordenada(BDTimes* bd_times);

#endif