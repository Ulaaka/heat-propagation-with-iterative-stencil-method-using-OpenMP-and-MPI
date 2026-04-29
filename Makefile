CFLAGS = -std=c99
MAKES = gccnearly, gcccomplete
all: $(MAKES)
gccnearly: heat.c main_nearly.c file_reader.c
	gcc -Ofast -fopenmp heat.c main_nearly.c file_reader.c -o heat_nearly -lm

gcccomplete: heat.c main_mpi.c file_reader.c
	mpicc -Ofast -fopenmp heat.c main_mpi.c file_reader.c -o heat_complete -lm