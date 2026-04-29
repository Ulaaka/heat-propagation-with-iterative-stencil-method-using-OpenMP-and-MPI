#!/bin/bash -l
#SBATCH -J heat
#SBATCH -p nodes
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -D ./
#SBATCH -t 00:20:00
#SBATCH -o slurm_%j.out
#SBATCH --export=ALL
module load gcc
module load openmpi

make gccnearly
make gcccomplete

AXIS_POINTS=$1
MAX_ITER=$2
CORES=${SLURM_CPUS_PER_TASK:-1}
export OMP_NUM_THREADS=$CORES

echo =========================================================
echo SLURM job: submitted date = `date`
date_start=`date +%s`
echo =========================================================
echo Job output begins
echo -----------------
echo

echo "OMP with $CORES threads"
srun -N 1 -n 1 ./heat_nearly $AXIS_POINTS $MAX_ITER input_${THREAD_TASK}.dat output_${THREAD_TASK}_${AXIS_POINTS}_${MAX_ITER}.dat

#echo "Started MPI with $RANK_MPI ranks"
#mpirun -np $RANK_MPI ./heat_complete 10 10 input_${RANK_MPI}.dat output_${RANK_MPI}_10_10.dat

echo
echo ---------------
echo Job output ends
date_end=`date +%s`
seconds=$((date_end-date_start))
minutes=$((seconds/60))
seconds=$((seconds-60*minutes))
hours=$((minutes/60))
minutes=$((minutes-60*hours))
echo =========================================================
echo SLURM job: finished date = `date`
echo Total run time : $hours Hours $minutes Minutes $seconds Seconds
echo =========================================================