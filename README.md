# ICE4131-HPC
Lab material for my ICE4131 module about High Performance Computing

## Lab 1

The aim of this tutorial is to introduce you to the GNU/Linux environment used by SuperComputing Wales. You will learn how to

- log in,
- write and
- compile code, and
- run your first parallel tasks using POSIX threads (also known as Pthreads).

[Click here to go to Lab 1](LAB1).

## Lab 2

The aim of this lab is to execute programs on the supercomputer using SLURM (Simple Linux Utility for Resource Management). You will:

- list the partitions that are available on the supercomputer
    ```bash
    $ sinfo
    ```
- launch “Jobs” using SLURM
    ```bash
    $ sbatch
    ```
- view information about jobs located in the Slurm scheduling queue.
    ```bash
    $ squeue -u $USER
    ```

[Click here to go to Lab 2](LAB2).

## Lecture 4

The code in this lecture shows how to parallelise serial code using Pthreads. It covers:

- Workload balancing of `N` tasks across `T` threads,
- Pthread creation,
- Callback execution,
- Joining threads,
- Evaluating execution time and speedup.

[Click here to go to Lecture 4](Lecture-4).

## Lab 3

In this lab, you practice what we've seen in the lecture so far:

1. Parallelise some serial code using Pthreads in `PthreadImage::flipHorizontally()` in [LAB3/src/PthreadImage.cxx](LAB3/src/PthreadImage.cxx),
2. Add timestamps to assess the execution time in [LAB3/src/flip.cxx](LAB3/src/flip.cxx),
3. Run the code on the compute nodes,
4. Plot graphs of runtimes and of speedups.

[Click here to go to Lab 3](LAB3).

## Lab 4

In this lab, you practice what we've seen in the lecture so far:

1. Finish parallelising some serial code using Pthreads in [LAB3/src/PthreadImage.cxx](LAB3/src/PthreadImage.cxx) (`PthreadImage::flipVertically()` and `PthreadImage::logFilter()`),
2. Add OpenMP pragmas to parallelise some serial code using OpenMP in [LAB3/src/OpenMPImage.cxx](LAB3/src/OpenMPImage.cxx) (`OpenMPImage::flipHorizontally()`, `OpenMPImage::flipVertically()` and `OpenMPImage::logFilter()`),
3. Compare the runtimes between Pthreads and OpenMP by plotting graphs of runtimes and of speedups.

[Click here to go to Lab 4](LAB4).

## Lecture 6

It is a step-by-step introduction to MPI. It covers:

- MPI initialisation,
- Getting the world size,
- Getting the process' rank,
- Getting the processor's name,
- MPI finalisation,
- How to execute MPI programs on the supercomputer using SLURM.

[Click here to go to Lecture 6](Lecture-6).
