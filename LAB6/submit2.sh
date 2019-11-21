#!/usr/bin/env bash
#
# Project/Account (use your own)
#SBATCH -A scw1563
#
# We ask for 40 tasks with 40 cores.
# We need one node, just for us.
#
# Number of tasks per node
#SBATCH --ntasks-per-node=40
#
# Number of cores per task
#SBATCH --cpus-per-task=1
#
# Use one node
#SBATCH --nodes=1
#
# Runtime of this jobs is less then 12 hours.
#SBATCH --time=12:00:00

# Clear the environment from any previously loaded modules
module purge > /dev/null 2>&1

# Load the module environment suitable for the job
source env.sh

# And finally run the job​
./run.sh

# End of submit file
