CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Lista de todos os arquivos fonte
SRCS = main.c analisador_lexico.c hashing.c

# Lista de todos os arquivos objeto
OBJS = $(SRCS:.c=.o)

# Nome do executável
EXEC = programa

# Dependências
DEPS = analisador_lexico.h hashing.h

# Regra de compilação
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run clean

run: $(EXEC)
	./$(EXEC) input.txt

clean:
	rm -f $(EXEC) $(OBJS)
