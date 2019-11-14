#!/usr/bin/env bash
# Example with 20 MPI tasks and 10 tasks per node.
#
# Project/Account (use your own)
#SBATCH -A scw1563
#
# Job name
#SBATCH --job-name="Chap7-HelloWorld-MPI"
#
# Number of MPI tasks
#SBATCH -n 20
#
# Number of tasks per node
#SBATCH --tasks-per-node=10
#
# Runtime of this jobs is less then 2 minutes.
#SBATCH --time=00:02:00

# Clear the environment from any previously loaded modules
module purge > /dev/null 2>&1

# Load the module environment suitable for the job
source env.sh

# And finally run the job​
mpirun ./bin/Chap7-HelloWorld-MPI

# End of submit file
