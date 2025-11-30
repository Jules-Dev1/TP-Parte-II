#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bd_partidas.h"

BDPartidas* bd_partidas_carregar(const char* filename) {
    FILE* arquivo = fopen(filename, "r");
    if (!arquivo) {
        printf("Erro ao abrir arquivo %s\n", filename);
        return NULL;
    }
    
    BDPartidas* bd = (BDPartidas*)malloc(sizeof(BDPartidas));
    if (!bd) {
        fclose(arquivo);
        return NULL;
    }
    
    bd->primeiro = NULL;
    bd->num_partidas = 0;
    
    char linha[256];
    fgets(linha, sizeof(linha), arquivo);
    
    while (fgets(linha, sizeof(linha), arquivo)) {
        Partida partida;
        linha[strcspn(linha, "\n")] = 0;
        
        char* token = strtok(linha, ",");
        if (token) partida.id = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) partida.time1_id = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) partida.time2_id = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) partida.gols_time1 = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) partida.gols_time2 = atoi(token);
        
        NoPartida* novo_no = (NoPartida*)malloc(sizeof(NoPartida));
        novo_no->partida = partida;
        novo_no->prox = bd->primeiro;
        bd->primeiro = novo_no;
        bd->num_partidas++;
    }
    
    fclose(arquivo);
    return bd;
}

void bd_partidas_liberar(BDPartidas* bd) {
    NoPartida* no = bd->primeiro;
    while (no != NULL) {
        NoPartida* proximo = no->prox;
        free(no);
        no = proximo;
    }
    free(bd);
}

int bd_partidas_proximo_id(BDPartidas* bd) {
    int max_id = -1;
    NoPartida* no = bd->primeiro;
    while (no != NULL) {
        if (no->partida.id > max_id) {
            max_id = no->partida.id;
        }
        no = no->prox;
    }
    return max_id + 1;
}

void bd_partidas_inserir(BDPartidas* bd, Partida partida) {
    partida.id = bd_partidas_proximo_id(bd);
    NoPartida* novo_no = (NoPartida*)malloc(sizeof(NoPartida));
    novo_no->partida = partida;
    novo_no->prox = bd->primeiro;
    bd->primeiro = novo_no;
    bd->num_partidas++;
}

int bd_partidas_remover(BDPartidas* bd, int id) {
    NoPartida* anterior = NULL;
    NoPartida* atual = bd->primeiro;
    
    while (atual != NULL) {
        if (atual->partida.id == id) {
            if (anterior == NULL) {
                bd->primeiro = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            bd->num_partidas--;
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return 0;
}

Partida* bd_partidas_buscar_por_id(BDPartidas* bd, int id) {
    NoPartida* no = bd->primeiro;
    while (no != NULL) {
        if (no->partida.id == id) {
            return &no->partida;
        }
        no = no->prox;
    }
    return NULL;
}

int bd_partidas_buscar_por_time(BDPartidas* bd, int time_id, int modo, Partida* resultados[]) {
    int count = 0;
    NoPartida* no = bd->primeiro;
    
    while (no != NULL && count < 100) {
        Partida* p = &no->partida;
        int match = 0;
        
        switch (modo) {
            case 1:
                match = (p->time1_id == time_id);
                break;
            case 2:
                match = (p->time2_id == time_id);
                break;
            case 3:
                match = (p->time1_id == time_id || p->time2_id == time_id);
                break;
        }
        
        if (match) {
            resultados[count++] = p;
        }
        no = no->prox;
    }
    
    return count;
}