# Campeonato Computacional de Futebol - Parte II

Sistema de gerenciamento de campeonato de futebol implementado em C, desenvolvido como trabalho prático para a disciplina de Estrutura de Dados. O sistema carrega dados de times e partidas de arquivos CSV, permitindo consultas, manutenção de dados e exibição da tabela de classificação ordenada.
Novas Funcionalidades (Parte II)

    Inserir Partida: Adiciona nova partida com ID automático

    Atualizar Partida: Modifica placar de partida existente

    Remover Partida: Exclui partida do sistema

    Tabela Ordenada: Classificação ordenada por PG, V e S

Estrutura do Projeto

campeonato-futebol/

├── main.c (Programa principal com menu completo)

├── time.h (TAD Time)

├── time.c (Implementação do TAD Time)

├── bd_times.h (TAD BDTimes com lista encadeada)

├── bd_times.c (Implementação do BDTimes)

├── partida.h (TAD Partida)

├── bd_partidas.h (TAD BDPartidas com lista encadeada)

├── bd_partidas.c (Implementação do BDPartidas)

├── Makefile (Script de compilação)

├── times.csv (Dados dos times)

├── partidas_vazio.csv (Cenário 1: sem partidas)

├── partidas_parcial.csv (Cenário 2: campeonato em andamento)

├── partidas_completo.csv (Cenário 3: campeonato completo)

└── README.md (Este arquivo)

Compilação e Execução
bash

# Compilar
make

# Executar
./campeonato

# Limpar
make clean

Como Usar

    Preparar arquivo de partidas:
    bash

cp partidas_parcial.csv partidas.csv

Executar o programa:
bash

./campeonato

    Menu de opções:

        1 - Consultar time por prefixo

        2 - Consultar partidas

        3 - Atualizar partida

        4 - Remover partida

        5 - Inserir partida

        6 - Imprimir tabela ordenada

        Q - Sair

TADs Principais
TAD Time

    Gerencia dados de um time (ID, nome, estatísticas)

    Operações: atualizar estatísticas, calcular pontos/saldo

TAD BDTimes

    Lista encadeada de times

    Operações: carregar, buscar, calcular estatísticas, imprimir tabela ordenada

TAD Partida

    Representa uma partida (ID, times, placar)

TAD BDPartidas

    Lista encadeada de partidas

    Operações: carregar, inserir, remover, atualizar, buscar

Decisões de Implementação

    Listas Encadeadas: Times e partidas armazenados em listas encadeadas

    ID Automático: Partidas recebem ID sequencial automaticamente

    Estatísticas Dinâmicas: Estatísticas recalculadas após cada alteração

    Ordenação: Tabela ordenada por PG (pontos), V (vitórias) e S (saldo)

    Confirmações: Operações críticas exigem confirmação do usuário

Cenários de Teste
bash

# Cenário 1: Campeonato vazio
cp partidas_vazio.csv partidas.csv
./campeonato

# Cenário 2: Campeonato parcial
cp partidas_parcial.csv partidas.csv  
./campeonato

# Cenário 3: Campeonato completo
cp partidas_completo.csv partidas.csv
./campeonato

Ambiente

    Sistema: Linux Ubuntu 22.04

    Compilador: GCC 11

    Linguagem: C

Desenvolvido por: Jules Santos Campos

Disciplina: Estrutura de Dados

Instituição: Instituto Federal do Espírito Santo - Campus Serra

Professor: Thiago M. Paixão
