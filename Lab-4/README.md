# ICE4131 - High Performance Computing (HPC)
## Lab 4: Pthreads versus OpenMP
### Tutor: Franck Vidal

## Objectives

In this lab, you practice what we've seen in the lecture so far:
1. Finish parallelising some serial code using Pthreads,
2. Add OpenMP pragmas to parallelise some serial code using OpenMP,
3. Compare the runtimes between Pthreads and OpenMP by plotting graphs of runtimes and of speedups.

Some code is provided for your convenience:
- `OpenMPImage` inherits of `Image`.
- `flip.cxx` and `log.cxx` are two programs making use of the serial code, phtread code, and OpenMP.
    - Usage: flip -- Flip the input image horizontally or vertically
```bash
        --horizontally
        -H
            Flip the image horizontally
        --vertically
        -V
            Flip the image vertically
        --num <n>
        -n <n>
            Number of threads/processes
        --implementation <string>
        -c <string>
            Choose implementation: serial|pthread|openmp|cuda|mpi
        --inputFile <fname>
        -i <fname>
            Input file to process
        --outputFile <fname>
        -o <fname>
            File to write
        --help
        -h
            Show help
```
    - Usage: log -- Apply a log filter on all the pixels of the input
```bash
        --num <n>
        -n <n>
            Number of threads/processes
        --implementation <string>
        -c <string>
            Choose implementation: serial|pthread|openmp|cuda|mpi
        --inputFile <fname>
        -i <fname>
            Input file to process
        --outputFile <fname>
        -o <fname>
            File to write
        --help
        -h
            Show help
```


<!-- Link to create images of code: https://carbon.now.sh -->


## Getting the code

1. Download the code from Blackboard. The file is `Lab4-20191030.tar.bz2`.
2. Copy this file from your PC to `hawklogin.cf.ac.uk` using WinSCP.
3. Connect to `hawklogin.cf.ac.uk` using a SSH client such as Putty.
4. Create a `LAB4` directory using `mkdir`.
5. Go into `LAB4` using the `cd` command.
6. Extract the archive using:
```bash
$ tar xjvfp ../Lab4-20191030.tar.bz2
```

## Compiling the environment

1. Reuse `env.sh` from [Lab 3](../Lab-3). It is used to load modules. You need:
```bash
module load cmake
module load gnuplot
module load compiler/gnu/9/2.0
```
**You need to do this EVERY TIME you log in.**

2. You can use
```bash
$ module list
```
to check that the modules are loaded.

3. Create a `bin` directory using `mkdir`.

4. Go into `bin` using the `cd` command.

5. Configure your project using CMake:
```bash
$ cmake ..
```

6. Compile your code
```bash
$ make
```


## Parallelise flipVertically() using Pthreads

See [Lab 3](../Lab-3) for a tutorial.

**Make sure you compile your code regularly!**

## Parallelise logFilter() using Pthreads

See [Lab 3](../Lab-3) for a tutorial.


## Parallelise logFilter() using OpenMP

```cxx
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        ...
    }
```


```cxx
#pragma omp parallel for num_threads(N)
    for (int i = 0; i < N; i++)
    {
        ...
    }
```

## Parallelise flipHorizontally() using OpenMP


## Parallelise flipVertically() using OpenMP

Do the same as for `flipHorizontally()`.
















## Run your program

1. To run your program, launch a job. DO NOT RUN IT DIRECTLY ON `hawklogin.cf.ac.uk`. Be nice to other users!
2. See [Lab 2](../Lab-2) for an explanation.
3. Create a new file named `submit.sh` containing:
```bash
#!/usr/bin/env bash
#

#SBATCH --job-name=my_test # Job name
#SBATCH --nodes=1                    # Use one node
#SBATCH --mem=600mb                  # Total memory limit
#SBATCH --time=00:15:00              # Time limit hrs:min:sec


# Apply the log filter using serial implementation
./log \
    -i ../../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o log_image-serial.txt \
    -c serial

# Apply the log filter using OpenMP implementation
./log \
    -i ../../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o log_image-openmp.txt \
    -n $SLURM_CPUS_PER_TASK \
    -c openmp

# Apply the log filter using Pthread implementation
./log \
    -i ../../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o log_image-pthread.txt \
    -n $SLURM_CPUS_PER_TASK \
    -c pthread

# Flip the image using serial implementation
./flip \
    -H  \
    -i ../../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o flip_image-serial.txt \
    -c serial

# Flip the image using OpenMP implementation
./flip \
    -H  \
    -i ../../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o flip_image-openmp.txt \
    -n $SLURM_CPUS_PER_TASK \
    -c openmp

# Flip the image using Pthread implementation
./flip \
    -H  \
    -i ../../LAB3/Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    -o flip_image-pthread.txt \
    -n $SLURM_CPUS_PER_TASK \
    -c pthread
```

4. To launch it, use:
```bash
$ sbatch  --account=scw1563 -c N submit.sh
```
**Note: replace N above with a number between 1 and 40.**

5. Wait for the job to complete. Use `squeue -u $USER`.

6. When the job is terminated, two new files should be there:
    - `output_image.txt` and
    - `slurm-%j.out`, with %j the job number.

7. Use `more slurm-%j.out` to see the content of the file.

8. To see the new image, download output file from `hawklogin.cf.ac.uk` to your PC using WinSCP.

9. Use ImageJ to visualise the image (Import->Text Image)
