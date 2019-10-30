#!/bin/bash
# Example with 20 MPI tasks and 10 tasks per node.
#
# Project/Account (use your own)
#SBATCH -A scw1563
#
# Number of MPI tasks
#SBATCH -n 20
#
# Number of tasks per node
#SBATCH --tasks-per-node=10
#
# Runtime of this jobs is less then 12 hours.
#SBATCH --time=12:00:00

# Clear the environment from any previously loaded modules
module purge > /dev/null 2>&1

# Load the module environment suitable for the job
module load mpi/openmpi/3.1.1

# And finally run the job​
mpirun ./bin/Chap6-HelloWorld-MPI-7

# End of submit file
