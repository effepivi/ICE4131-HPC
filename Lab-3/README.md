# ICE4131 - High Performance Computing (HPC)
## Lab 3: Parallelisation using Pthread
### Tutor: Franck Vidal

## Objectives

In this lab, you practice what we've seen in the lecture so far:
1. Parallelise some serial code using Pthreads,
2. Add timestamps to assess the execution time,
3. Run the code on the compute nodes
4. Plot graphs of runtimes and of speedups.

<!-- Link to create images of code: https://carbon.now.sh -->


## Getting the code

1. Download the code from Blackboard
2. Copy the code from your PC to `hawklogin.cf.ac.uk` using WinSCP
3. Connect to `hawklogin.cf.ac.uk` using a SSH client such as Putty.
4. Create a `LAB3` directory using `mkdir`.
5. Go into `LAB3` using the `cd` command.
6. Extract the archive using:
```bash
$ tar xjvfp ../Lab3-20191017.tar.bz2
```


## Compiling the environment

1. Create a script to set your environment, e.g. `env.sh`. It should contain:
```bash
module load cmake
module load gnuplot
module load compiler/gnu/9/2.0
```
2. Set your environment using:
```bash
$ source env.sh
$
```
3. You can use
```bash
$ module list
```
to check that the modules are loaded.
4. Create a `bin` directory using `mkdir`.
5. Go into `bin` using the `cd` command.
6. Configure your project using CMake:
```bash
$ cmake ..
```
7. Compile your code
```bash
$ make
```


## Run your program

1. See Lab 2.
2. Download ASCII image file from Blackboard.
3. Copy the image from your PC to `hawklogin.cf.ac.uk` using WinSCP. Make sure the file is in LAB3.
4. Uncompress the image:
```bash
$ bunzip2 ../Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt.bz2
```
5. Create a new file named `submit.sh` containing:
```bash
#!/bin/bash
#

#SBATCH --job-name=my_test # Job name
#SBATCH --nodes=1                    # Use one node
#SBATCH --mem=600mb                  # Total memory limit
#SBATCH --time=00:15:00              # Time limit hrs:min:sec

./flip \
    -H  \
    ../Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt \
    output_image.txt \
    $SLURM_CPUS_PER_TASK
```
6. To launch it, use:
```bash
$ sbatch  --account=scw1563 -c N submit.sh
```
**Note: replace N above with a number between 1 and 40.**
7. Wait for the job to complete. Use `squeue -u $USER`.
8. When the job is terminated, two new files should be there:
    - `output_image.txt` and
    - `slurm-%j.out`, with %j the job number.
9. Use `more slurm-%j.out` to see the content of the file.
10. To see the new image, download `output_image.txt` from `hawklogin.cf.ac.uk` to your PC using WinSCP.
11. Use ImageJ to visualise the image (Import->Text Image)

## Add timestamps

1. Edit the file `../src/flip.cxx` to add timestamps. You can use `nano`.
2. See examples in [https://github.com/effepivi/ICE4131-HPC/blob/master/Lecture-4/chapter4-2.cxx](https://github.com/effepivi/ICE4131-HPC/blob/master/Lecture-4/chapter4-2.cxx).
3. You want to display how it takes to flip the image horizonatally and vertically, with and without threads.
