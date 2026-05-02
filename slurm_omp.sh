#!/bin/bash -l
#SBATCH -J heat_omp
#SBATCH -p course
#SBATCH -D ./
#SBATCH --export=ALL
#SBATCH -o slurm_nearly_%j.out

module purge
module load gcc
module load openmpi

make gccnearly
export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1}

echo "OpenMP with $OMP_NUM_THREADS threads"
# Example run: sbatch -c 32 slurm_omp.sh heat_nearly 512 1000000 input_1.dat output_1_512_1000000.dat
srun ./$1 $2 $3 $4 $5
