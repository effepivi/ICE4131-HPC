#!/bin/bash
# Example with 20 MPI tasks and 20 tasks per node.
#
# Project/Account (use your own)
#SBATCH -A scw1563
#
# Number of MPI tasks
#SBATCH -n 20
#
# Number of tasks per node
#SBATCH --tasks-per-node=20
#
# Runtime of this jobs is less then 12 hours.
#SBATCH --time=12:00:00

# Clear the environment from any previously loaded modules
module purge > /dev/null 2>&1

# Load the module environment suitable for the job
module load compiler/gnu/8/1.0 mpi/openmpi/3.1.1 gnuplot


# And finally run the job​
mpirun ./bin/flip -H \
    -i ../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o flip-H-mpi.txt \
    -c MPI

# End of submit file
