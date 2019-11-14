#!/usr/bin/env bash
# Example with 200 MPI tasks and 40 tasks per node.
#
# Project/Account (use your own)
#SBATCH -A scw1563
#
# Number of MPI tasks
#SBATCH -n 200
#
# Number of tasks per node
#SBATCH --tasks-per-node=40
#
# Runtime of this jobs is less then 2 minutes.
#SBATCH --time=00:02:00
#
#SBATCH --job-name="Chap7-Comm4"
#
#SBATCH --output=Chap7-Comm4.out
#
#SBATCH --error=Chap7-Comm4.err
#

# Clear the environment from any previously loaded modules
module purge > /dev/null 2>&1

# Load the module environment suitable for the job
source env.sh

# And finally run the job​
mpirun ./bin/Chap7-Comm4-MPI 2> /dev/null

# End of submit file
