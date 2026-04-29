
gccnearly: heat.c main_nearly.c file_reader.c
	gcc -fopenmp heat.c main_nearly.c file_reader.c -o heat_nearly -lm

gcccomplete: heat.c main_mpi.c file_reader.c
	mpicc heat.c main_mpi.c file_reader.c -o heat_complete -lm