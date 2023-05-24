# Nome do programa
EXEC = jewels

# Arquivos .o + main.o
OBJS =	./obj/game.o \
		./obj/allegro.o \
		./obj/jewels.o	

# Opções de compilação
CC = gcc
FLAGS = -Wall -std=c99
ALLEGRO = 	-lallegro_image -lallegro_dialog -lallegro_primitives -lallegro_ttf \
			-lallegro_font -lallegro_audio -lallegro_acodec -lallegro -lm

# Compilar e Linkar
all: objFolder $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(FLAGS) $^ -o $@ $(ALLEGRO)

./obj/%.o: ./%.c
	$(CC) $(FLAGS) -c $^ -o $@

objFolder:
	@mkdir -p obj

clean: 
	@rm -rf ./obj/*o *~
	@rm -df obj 

purge: clean
	@rm -rf $(EXEC)
	@rm -rf *.txt
