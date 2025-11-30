CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
OBJS = main.o time.o bd_times.o bd_partidas.o
TARGET = campeonato

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c bd_times.h bd_partidas.h time.h
	$(CC) $(CFLAGS) -c main.c

time.o: time.c time.h
	$(CC) $(CFLAGS) -c time.c

bd_times.o: bd_times.c bd_times.h time.h bd_partidas.h
	$(CC) $(CFLAGS) -c bd_times.c

bd_partidas.o: bd_partidas.c bd_partidas.h partida.h
	$(CC) $(CFLAGS) -c bd_partidas.c

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean