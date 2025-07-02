# Nome do executavel
EXECUTAVEL = malha

# Diretórios
SRC_DIR = ./src
HEADER_DIR = ./header
OBJ_DIR = ./objects

# Arquivos fonte .c
C_SOURCE = $(wildcard $(SRC_DIR)/*.c)

# Arquivos header .h
H_SOURCE = $(wildcard $(HEADER_DIR)/*.h)

# Arquivos objeto .o
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCE))

# libs
LIBS = -lm

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -O2 -march=native -g3 -std=c99

# Comando para remover arquivos
RM = rm -rf

# Target padrão
all: obj $(EXECUTAVEL)

# Criação do diretório de objetos
obj:
	@ mkdir -p $(OBJ_DIR)

# Linkagem dos objetos para gerar o executável
$(EXECUTAVEL): $(OBJ)
	@ echo ' '
	@ echo 'Building...'
	@ $(CC) $^ -o $@ $(LIBS)
	@ echo 'Finished building...'
	@ echo ' '

# Regra para compilar cada arquivo .c em um arquivo .o correspondente
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(H_SOURCE)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos gerados
clean:
	@ echo ' '
	@ $(RM) $(OBJ_DIR)/*.o $(EXECUTAVEL) *~
	@ rmdir $(OBJ_DIR)
	@ echo 'Finished clean...'
	@ echo ' '

.PHONY: all clean
