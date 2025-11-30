#include "time.h"

void time_atualizar_estatisticas(Time* time, int gols_feitos, int gols_sofridos) {
    time->gols_marcados += gols_feitos;
    time->gols_sofridos += gols_sofridos;
    
    if (gols_feitos > gols_sofridos) {
        time->vitorias++;
    } else if (gols_feitos < gols_sofridos) {
        time->derrotas++;
    } else {
        time->empates++;
    }
}

int time_calcular_pontos(const Time* time) {
    return (time->vitorias * 3) + time->empates;
}

int time_calcular_saldo(const Time* time) {
    return time->gols_marcados - time->gols_sofridos;
}