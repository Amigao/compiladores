CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Lista de todos os arquivos fonte
SRCS = main.c lexical_analyzer.c sintatic_analyzer.c hashing.c errors_management.c

# Lista de todos os arquivos objeto
OBJS = $(SRCS:.c=.o)

# Nome do executável
EXEC = programa

# Dependências
DEPS = aux.h lexical_analyzer.h sintatic_analyzer.h hashing.h errors_management.h 

# Regra de compilação
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run clean

# Regra para executar o programa, aceitando um arquivo de entrada e usando input.txt caso não haja entrada do usuario
run: $(EXEC)
ifeq ($(ARGS),)
	./$(EXEC) input.txt
else
	./$(EXEC) $(ARGS)
endif

clean:
ifeq ($(OS),Windows_NT)
	del /F /Q $(EXEC) $(OBJS)
else
	rm -f $(EXEC) $(OBJS)
endif