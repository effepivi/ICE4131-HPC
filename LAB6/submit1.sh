#!/usr/bin/env bash
#
# Project/Account (use your own)
#SBATCH -A scw1563
#SBATCH --job-name=my_test # Job name
#SBATCH --nodes=1                    # Use one node
#SBATCH --mem=600mb                  # Total memory limit
#SBATCH --time=00:15:00              # Time limit hrs:min:sec


# Apply the log filter using serial implementation
./log \
    -i ../../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o log_image-serial.txt \
    -c serial

# Apply the log filter using OpenMP implementation
./log \
    -i ../../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o log_image-openmp.txt \
    -n $SLURM_CPUS_PER_TASK \
    -c openmp

# Apply the log filter using Pthread implementation
./log \
    -i ../../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o log_image-pthread.txt \
    -n $SLURM_CPUS_PER_TASK \
    -c pthread

# Apply the log filter using MPI implementation
./log \
    -i ../../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o log_image-MPI.txt \
    -n $SLURM_CPUS_PER_TASK \
    -c MPI

# Flip the image using serial implementation
./flip \
    -H  \
    -i ../../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o flip_image-serial.txt \
    -c serial

# Flip the image using OpenMP implementation
./flip \
    -H  \
    -i ../../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o flip_image-openmp.txt \
    -n $SLURM_CPUS_PER_TASK \
    -c openmp

# Flip the image using Pthread implementation
./flip \
    -H  \
    -i ../../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o flip_image-pthread.txt \
    -n $SLURM_CPUS_PER_TASK \
    -c pthread

# Flip the image using MPI implementation
./flip \
    -H  \
    -i ../../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o flip_image-MPI.txt \
    -n $SLURM_CPUS_PER_TASK \
    -c MPI
