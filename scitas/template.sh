#!/bin/bash
#SBATCH -n={number_agents}
#SBATCH --output=/scratch/gebauer/ParallelCG/out/{problem_name}.out
#SBATCH --constraint=E5v2
{reservation}

echo "Strong Skaling with 1 processor"

# load modules
module purge
module load gcc/5.4.0
module load mvapich2/2.2
module load cmake
module load python/3.6.1

# create working directory
mkdir /scratch/gebauer/ParallelCG/run/{problem_name}

# go into my directory
cd /scratch/gebauer/ParallelCG/run/{problem_name}


# run for N={problem_size}
python3 /scratch/gebauer/ParallelCG/generate.py -N {problem_size} -A mat.npy -v vec.npy
srun /scratch/gebauer/ParallelCG/CGMethod -M mat.npy -v vec.npy -x sol.npy
python3 /scratch/gebauer/ParallelCG/validate.py -A mat.npy -v vec.npy -x sol.npy
rm mat.npy vec.npy sol.npy

