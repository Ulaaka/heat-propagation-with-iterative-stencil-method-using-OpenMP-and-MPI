CFLAGS = -std=c99
MAKES = gccnearly, gcccomplete

TARGET_NEARLY=heat.c main_nearly.c file_reader.c
TARGET_COMPLETE=heat.c main_mpi.c file_reader.c

all: $(MAKES)
gccnearly: $(TARGET_NEARLY)
	gcc -Ofast -fopenmp $(TARGET_NEARLY) -o heat_nearly -lm

gcccomplete: $(TARGET_COMPLETE)
	mpicc -Ofast -fopenmp $(TARGET_COMPLETE) -o heat_complete -lm