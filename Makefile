# Grupo: 
# Beatriz Matias Santana Maia: 2019107651
# Luana Gabriele de Sousa Costa: 2018102324 
# Sophie Dilhon Gama: 2019107591

CC		:= gcc
CFLAGS	:= 

BIN		:= bin
SRC		:= src
INCLUDE	:= src

EXECUTABLE	:= vsh
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)

VALGRIND	:= 

CINCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
SOURCES		:= $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))
OBJECTS		:= $(SOURCES:.c=.o)

all: clean $(EXECUTABLE)

.PHONY: clean
clean:
	-$(RM) $(OBJECTS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(CINCLUDES) $^ -o $@ $(LIBRARIES)

# Comandos para executar
run: 
	./$(EXECUTABLE) 

# Comandos para executar com valgrind
val: 
	valgrind --leak-check=full ./$(EXECUTABLE) 