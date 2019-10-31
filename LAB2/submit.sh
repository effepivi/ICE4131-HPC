#!/bin/bash
#
#SBATCH --job-name=my_test # Job name
#SBATCH --account=scw1563
#SBATCH --ntasks=1                   # Run a single task
#SBATCH --mem=600mb                  # Total memory limit
#SBATCH --time=00:15:00              # Time limit hrs:min:sec

echo HOSTNAME:
hostname

echo CONTAIN OF HOME DIRECTORY
ls $HOME
echo WAIT 15 seconds
sleep 15s
