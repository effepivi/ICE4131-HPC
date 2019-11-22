#!/usr/bin/env bash
#
#SBATCH -A scw1563                   # Project/Account (use your own)
#SBATCH --job-name=my_test           # Job name
#SBATCH --nodes=1                    # Use one node
#SBATCH --cpus-per-task=10           # Number of cores per task
#SBATCH --mem=600mb                  # Total memory limit
#SBATCH --time=00:15:00              # Time limit hrs:min:sec
#SBATCH --gres=gpu:2                 # We ask for a GPU
#SBATCH -p gpu

# Clear the environment from any previously loaded modules
module purge > /dev/null 2>&1

# Load the module environment suitable for the job
source env.sh

# Apply the log filter using serial implementation
./bin/log \
    -i ../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o log_image-serial.txt \
    -c serial

# Apply the log filter using OpenMP implementation
./bin/log \
    -i ../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o log_image-openmp.txt \
    -n $SLURM_CPUS_PER_TASK \
    -c openmp

# Apply the log filter using Pthread implementation
./bin/log \
    -i ../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o log_image-pthread.txt \
    -n $SLURM_CPUS_PER_TASK \
    -c pthread

# Apply the log filter using MPI implementation
mpirun -np $SLURM_CPUS_PER_TASK \
    ./bin/log \
    -i ../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o log_image-MPI.txt \
    -c MPI

# Apply the log filter using CUDA implementation
./bin/log \
    -i ../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o log_image-cuda.txt \
    -c cuda


# Flip the image using serial implementation
./bin/flip \
    -H  \
    -i ../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o flip_image-serial.txt \
    -c serial

# Flip the image using OpenMP implementation
./bin/flip \
    -H  \
    -i ../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o flip_image-openmp.txt \
    -n $SLURM_CPUS_PER_TASK \
    -c openmp

# Flip the image using Pthread implementation
./bin/flip \
    -H  \
    -i ../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o flip_image-pthread.txt \
    -n $SLURM_CPUS_PER_TASK \
    -c pthread

# Flip the image using MPI implementation
mpirun -np $SLURM_CPUS_PER_TASK \
    ./bin/flip \
    -H  \
    -i ../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o flip_image-MPI.txt \
    -c MPI

# Flip the image using CUDA implementation
./bin/flip \
    -H  \
    -i ../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o flip_image-cuda.txt \
    -c CUDA

