#!/usr/bin/env bash
#
# Project/Account (use your own)
#SBATCH -A scw1563
#
# We ask for 1 tasks with 1 core only.
# We ask for a GPU
#SBATCH --gres=gpu:2
#SBATCH -p gpu
#
# Number of tasks per node
#SBATCH --ntasks-per-node=1
#
# Number of cores per task
#SBATCH --cpus-per-task=1
#
# Use one node
#SBATCH --nodes=1
#
# Runtime of this jobs is less then 15 minutes.
#SBATCH --time=00:15:00

# Clear the environment from any previously loaded modules
module purge > /dev/null 2>&1

# Load the module environment suitable for the job
source env.sh

# And finally run the job​
./run.sh
#./bin/flip   -H  -c CUDA -i "../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt" -o test.txt

# End of submit file
