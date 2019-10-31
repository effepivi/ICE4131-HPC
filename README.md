# ICE4131-HPC
Lab material for my ICE4131 module about High Performance Computing

## Lab 1

The aim of this tutorial is to introduce you to the GNU/Linux environment used by SuperComputing Wales. You will learn how to

- log in,
- write and
- compile code, and
- run your first parallel tasks using POSIX threads (also known as Pthreads).


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
