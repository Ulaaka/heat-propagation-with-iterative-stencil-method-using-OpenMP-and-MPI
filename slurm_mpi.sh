#!/bin/bash -l
#SBATCH -p nodes
#SBATCH -c 8
#SBATCH -o slurm_complete_%j.out
#SBATCH -D ./
#SBATCH --export=ALL

module purge
module load intel/oneapi-hpc-toolkit-2025.1.3.10
module load mpi/2021.15

make gcccomplete
procs=${SLURM_NTASKS:-1}
export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1}

echo "MPI with $procs ranks"
#Example run: sbatch -n 32 slurm_mpi.sh heat_complete 64 10000 input_32.dat output_32_64_10000.dat
mpirun -np $procs ./$1 $2 $3 $4 $5
