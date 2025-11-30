#ifndef BD_PARTIDAS_H
#define BD_PARTIDAS_H

#include "partida.h"

typedef struct no_partida {
    Partida partida;
    struct no_partida* prox;
} NoPartida;

typedef struct {
    NoPartida* primeiro;
    int num_partidas;
} BDPartidas;

BDPartidas* bd_partidas_carregar(const char* filename);
void bd_partidas_liberar(BDPartidas* bd);
void bd_partidas_inserir(BDPartidas* bd, Partida partida);
int bd_partidas_remover(BDPartidas* bd, int id);
Partida* bd_partidas_buscar_por_id(BDPartidas* bd, int id);
int bd_partidas_buscar_por_time(BDPartidas* bd, int time_id, int modo, Partida* resultados[]);
int bd_partidas_proximo_id(BDPartidas* bd);

#endif