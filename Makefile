SRC := src

all:
	-gcc -c ./${SRC}/game_variables.c -o ./${SRC}/game_variables.o
	-gcc -c ./${SRC}/toml.c -o ./${SRC}/toml.o
	-gcc -c ./${SRC}/script_loader.c -o ./${SRC}/script_loader.o
	-gcc -c ./${SRC}/render.c -o ./${SRC}/render.o
	-gcc ./${SRC}/main.c ./${SRC}/toml.o ./${SRC}/game_variables.o ./${SRC}/script_loader.o ./${SRC}/render.o -o engine -lncurses

clean:
	rm -rf ./${SRC}/*.o
	rm -rf engine
	rm -rf game.log