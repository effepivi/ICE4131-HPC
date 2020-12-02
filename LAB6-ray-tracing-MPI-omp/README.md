---
title: Lab 6 -- Parallelisation using OpenMP and MPI
author: Dr Franck P. Vidal
subtitle: ICE4131 - High Performance Computing (HPC)
date: Week 10
keywords: ICE4131, High Performance Computing, HPC, C/C++, ray tracing, Pthread, OpenMP, MPI, CUDA, Bangor University, School of Computer Science and Electronic Engineering
institute: School of Computer Science and Electronic Engineering, Bangor University
---

# Objectives

I assume you finished the Pthread code, the OpenMP code, the MPI code and that you ran them using various numbers of threads and processes. In today's lab, you'll use combine the use of OpenMP and MPI:

# Getting the latest version of the code

1. Go to your directory on Hawklogin where the code is.
2. In case I changed some of the code or added new file, update the repository using:
```bash
$ git pull
$
```

I added a new file (`submit-mpi-omp.sh`) and change one (`createTiming.sh`).

# Loading the modules

Same old story. Load:

- cmake
- compiler/intel/2020/2
- mpi/intel


# Add the new executable in `CMakeLists.txt`

1. Add the new executable
```cmake
add_executable(main-mpi-omp src/main-mpi-omp.cxx)
```
2. Specify extra header directories
```cmake
TARGET_INCLUDE_DIRECTORIES(main-mpi-omp PUBLIC ${ASSIMP_INCLUDE_DIRS} ${MPI_INCLUDE_PATH})
```
3. The linkage
```cmake
TARGET_LINK_LIBRARIES (main-mpi-omp PUBLIC RayTracing ${ASSIMP_LIBRARY} ${MPI_CXX_LIBRARIES} OpenMP::OpenMP_CXX)
```

# Parallelise the code using OpenMP

Copy/Paste the content of `main-mpi.cxx` into `main-mpi-omp.cxx`.
Use a `pragma` to parallelise a for loop. See [../LAB4-ray-tracing-omp](../LAB4-ray-tracing-omp).
Compile.

# Run your program

1. To run your program, launch a job. DO NOT RUN IT DIRECTLY ON `hawklogin.cf.ac.uk`. Be nice to other users!
2. See [Lab 2](../LAB2) for an explanation.
3. I provided a script, [`submit-mpi-omp.sh`](../../SimpleRayTracing/submit-mpi-omp.sh). Edit this file to use your email address in `echo "##SBATCH --mail-user=YOUREMAILADDRESS@bangor.ac.uk`.
`submit-mpi.sh` creates another 4*8=32 scripts `submit-mpi-*-*.sh` and submit the jobs with 1, 2, 3, and 4 nodes with 1, 4, 8, 16, 24, 40, 80 and 160 threads on each node.

For example, the script below `submit-mpi-omp-4-40.sh` is the script used to submit a job with 40 threads on 4 a total of 160 threads.

```bash
#!/usr/bin/env bash
#
#SBATCH -A scw1563                   # Project/Account (use your own)
##SBATCH --mail-user=YOUREMAILADDRESS@bangor.ac.uk  # Where to send mail
#SBATCH --mail-type=END,FAIL         # Mail events (NONE, BEGIN, END, FAIL, ALL)
#SBATCH --job-name=RT-MPI-omp-4-40       # Job name
#SBATCH --output ray_tracing-%j.out  #
#SBATCH --error ray_tracing-%j.err   #
#SBATCH --nodes=4                    # Use one node
#SBATCH --ntasks-per-node=1         # Number of tasks per node
#SBATCH --cpus-per-task=40            # Number of cores per task
#SBATCH --time=00:25:00              # Time limit hrs:min:sec
#SBATCH --mem=600mb                  # Total memory limit
thread_number=40
module purge > /dev/null 2>&1
module load cmake mpi/intel
COMPILER="gcc (GCC) 4.8.5 20150623 (Red Hat 4.8.5-39)"
COMPILER="icc (ICC) 18.0.2 20180210"
TEMP=`lscpu|grep "Model name:"`
IFS=':' read -ra CPU_MODEL <<< "$TEMP"
width=2048
height=2048
echo Run ./main-mpi with 40 processes.
export OMP_NUM_THREADS=40
/usr/bin/time --format='%e' mpirun  ./bin/main-mpi-omp --size 2048 2048 --jpeg mpi-omp-4-40-2048x2048.jpg 2> temp-mpi-4-40
RUNTIME=`cat temp-mpi-4-40`
echo ${CPU_MODEL[1]},MPI-omp,$thread_number,4,$COMPILER,${width}x$height,$RUNTIME >> timing-mpi-omp-4-40.csv
#rm temp-mpi-4-40
```


4. To launch `submit-mpi-omp.sh`, use:
```bash
$ ./submit-mpi-omp.sh
```
5. Wait for the job to complete. Use `squeue -u $USER`.

6. When the job is terminated, you'll have new files. Examine their content. Are the JPEG files as expected?

7. To see the new images, download them from `hawklogin.cf.ac.uk` to your PC using WinSCP.

8. Only go to the next section when everything works as expected. If not, debug your code.

# Performance evaluation

Same old story. But I haven't doe it myself. Let's see what you can get ;-)
