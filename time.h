#ifndef TIME_H
#define TIME_H

typedef struct {
    int id;
    char nome[50];
    int vitorias;
    int empates;
    int derrotas;
    int gols_marcados;
    int gols_sofridos;
} Time;

void time_atualizar_estatisticas(Time* time, int gols_feitos, int gols_sofridos);
int time_calcular_pontos(const Time* time);
int time_calcular_saldo(const Time* time);

#endif