# ICE4131 - High Performance Computing (HPC)
## Week 3: Performance assessment of a program using gprof
### Tutor: Franck Vidal

## Objectives

In this lab, you'll practice what we've seen in the lecture so far:

1. Identify the portion of the code that is slow using `gprof`.
2. Run the code on the compute nodes with output images of increasing sizes.
3. Plot graphs of runtimes for the program compiled with `g++` and `icc`

Some code is provided for your convenience. It is available on GitHub at [https://github.com/effepivi/SimpleRayTracing](https://github.com/effepivi/SimpleRayTracing).
- There is the serial code of a simple ray tracer in `src/main.cxx`.
- You're not expected to write any C/C++ code this week.
- You'll submit jobs using SLURM and create a spreadsheet.

<!-- Link to create images of code: https://carbon.now.sh -->


## Getting the code

1. Log on `hawklogin.cf.ac.uk` using your favourite SSH client, e.g. [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty) or `powershell` on MS Windows.
2. If you haven't done it yet, download the code from GitHub using
```bash
$ git clone https://github.com/effepivi/SimpleRayTracing.git
$
```
3. Go into the directory that contains the code:
```bash
$ cd SimpleRayTracing
$
```
4. If you already downloaded the code from GitHub
  - Go into the directory containing the files of this repository.
  - In case I changed the code, update the files using:
  ```bash
  $ git pull
  $
  ```

## Compiling environments

1. Create two scripts to set your environment. One for the GNU C++ compiler (`g++`); one for Intel's (`icc`). In the current directory (which should be `SimpleRayTracing`), create
    - `env-gnu.sh` that contains
    ```bash
    module purge > /dev/null 2>&1
    module load cmake
    module load gnuplot
    module load compiler/gnu/9/2.0
    ```
    - `env-intel.sh` that contains
    ```bash
    module purge > /dev/null 2>&1
    module load cmake
    module load gnuplot
    module load compiler/intel/2020/4
    ```

2. Create a directory for binaries compiled with each compiler using `mkdir`, e.g.

```bash
$ mkdir bin-gnu
$ mkdir bin-intel
$
```

## Profiling with the GNU compiler

1. Set your environment to binaries with the GNU compiler

```bash
$ source env-gnu.sh
$
```
**You'll need to run this command EVERY TIME you log in, or every time you want to change compiler.** We often forget.

2. To check that the modules are loaded, you can use

```bash
$ module list
$
```

3. Go into the appropriate `bin` directory, i.e. `bin-gnu`, using the `cd` command

```bash
$ cd bin-gnu
$
```

4. Configure your project using CMake

```bash
$ cmake ..
$
```

`..` is the parent directory of the current directory. CMake is looking for the `CMakeLists.txt` file.

5. Build the project

```bash
$ make
$
```

It's going to take ages to start with as it's building 3rd party libraries to load 3D models.

6. Enable profiling: By default CMake will enable the `Release` mode for producing code that is fast. You don't want that, at least for now, as we need to assess the program to know what is slow.

    - Run  `ccmake .` (yes, two `c`). (One or two dots, it does not matter as CMake already knows where `CMakeLists.txt` is).
    ```bash
    $ ccmake .
    $
    ```
    - Change the `CMAKE_BUILD_TYPE` variable to `Debug`
        - Use the arrows to go to the right line.
        - Press `ENTER` to edit.
        - Type `Debug`
        - Press `ENTER` to save the change.

    - Press `t` to "toggle".
    - In the `CMAKE_CXX_FLAGS_DEBUG` variable, add the `-pg` option. It will enable the profiling.
    - Press `c` to "configure".
    - Press `g` to "generate" the project.

7. Compile again. Only the ray-tracer will compile, not the 3rd party libraries. You'll see, it'll be fast this time.

```bash
$ make
$
```

- The program can use command line arguments:

```bash
Usage: ./main <option(s)>
Options:
	-h,--help                       Show this help message
	-s,--size IMG_WIDTH IMG_HEIGHT  Specify the image size in number of pixels (default values: 2048 2048)
	-b,--background R G B           Specify the background colour in RGB, acceptable values are between 0 and 255 (inclusive) (default values: 128 128 128)
	-j,--jpeg FILENAME              Name of the JPEG file (default value: test.jpg)
```

8. Submit a job using `sbatch`. I created a set of scripts for you. Have a look at `submit-serial-gnu.sh` in a text editor. You need to edit it. For example you can change the email address to use yours (also remove one of  the `#` characters).

```
$ cd ..
$ nano submit-serial-gnu.sh
$ sbatch submit-serial-gnu.sh
$ squeue -u $USER
$
```

It will create a file called `gmon.out` in your current directory (`SimpleRayTracing`).

9. Wait your the job to complete. In the meantime, have a look at [https://sourceware.org/binutils/docs/gprof/](https://sourceware.org/binutils/docs/gprof/) for information about how to use the profiler.

10. Analyse the results using:

```bash
$ gprof bin-gnu/main > serial-profiling.txt
$ nano serial-profiling.txt
$
```

11. Identify which steps took the longest. **Make sure you document what your results are as you'll need them in your assignment.**

## Turn off debugging and profiling

1. Set your environment to binaries with the GNU compiler

```bash
$ source env-gnu.sh
$
```

3. Go into the appropriate `bin-gnu`, using the `cd` command

```bash
$ cd bin-gnu
$
```

4. Configure your project using CMake

```bash
$ ccmake ..
$
```

5. Change the `CMAKE_BUILD_TYPE` variable to `Release`, configure and generate.

6. Build the project

```bash
$ make
$
```

7. Configure and build the project with the Intel compiler

```bash
$ cd ..
$ source env-intel.sh
$ cmake ..
$ make
$ cd ..
$
```

## Run your program with various image sizes

We aim to assess the performance of the Intel compiler against the GNU compiler. We will generate images with two programs we created, with different image sizes.

### 128x128

- Submit the jobs

```bash
$ sbatch submit-serial-gnu.sh
$ sbatch submit-serial-intel.sh
$
```

### 256x256

- Edit `submit-serial-gnu.sh`

```bash
$ nano submit-serial-gnu.sh
```

- Find the width and the height of the image in the script and change the values accordingly.

- Submit the job

```bash
$ sbatch submit-serial-gnu.sh
$
```

- Edit `submit-serial-intel.sh`

```bash
$ nano submit-serial-intel.sh
```

- Find the width and the height of the image in the script and change the values accordingly.

- Submit the job

```bash
$ sbatch submit-serial-intel.sh
$
```

### 512x512, 1024x1024, 2048x2048, 4096x4096

Do the same but with other image sizes.

### Create the spreadsheet

- Once the jobs start to complete, you can observe the results

```bash
$ cat timing.csv timing-serial-intel-*.csv timing-serial-gnu-*.csv > runtime.csv
$
```

- Download `runtime.csv` using your preferred SFTP client, e.g. WinSCP on the lab machines.
- Open the file using MS Excel or equivalent.
- For each image resolution, which compiler provided the fastest computations?
- What is your conclusion?

<!--



## Notes

I made some changes to the program on the 28th of Nov:

- The rendering loop is now in a function (that'll be better for profiling).
**Question:** Why?

**Answer:** When you edit your program and submit a job, you can run small tasks (nicer for debugging). -->

<!--
## Run your program

1. To run your program, launch a job. DO NOT RUN IT DIRECTLY ON `hawklogin.cf.ac.uk`. Be nice to other users!
2. See Lab 2.
3. Download the polygon mesh from Blackboard: `dragon.ply`
4. Download the texture file from Blackboard: `Bangor_Logo_A1.jpg`
5. Copy the files from your PC to `hawklogin.cf.ac.uk` using your favourite SCP client, e.g. [WinSCP](https://winscp.net/) or [FileZilla](https://filezilla-project.org/). Make sure you put the files in the same directory as `CMakeLists.txt`.
6. Create a new file named `submit-serial.sh` containing:
```bash
#!/usr/bin/env bash
#

#SBATCH --job-name=my_test # Job name
#SBATCH --nodes=1                    # Use one node
#SBATCH --mem=600mb                  # Total memory limit
#SBATCH --time=01:00:00              # Time limit hrs:min:sec

./main --jpeg test.jpg -b 128 0 0 -s 512 256
```
7. To launch it, use:
```bash
$ sbatch  --account=scw1563 submit-serial.sh
```
8. Wait for the job to complete. Use `squeue -u $USER`.
9. When the job is terminated, three new files should be there:
    - `test.jpg`,
    - `slurm-%j.out`, with %j the job number, and
    - `slurm-%j.err`, with %j the job number.
10. Use `more slurm-%j.out` to see the content of the file.
11. To see the new image, download `test.jpg` from `hawklogin.cf.ac.uk` to your PC using your favourite SCP client.
12. Use an image viewer to visualise the image.

<!--
## Add timestamps

1. Edit the file `../src/main.cxx` to add timestamps. You can use `nano`, `vi` or `emacs`.
2. See examples in [chapter4-2.cxx](../Lecture-4/chapter4-2.cxx).
3. You want to display how long it takes to perform the main steps in the main function, with and without threads.
    - Identify all the main steps.
    - Add timestamps.
    - Print the durations in the terminal.
4. Compile
5. Submit your job.
6. Use `more` to visualise the content of the `slurm-%j.out` file corresponding to the job.
7. Identify which step took the longest.

## Profiling

1. Run `ccmake .` and change the `CMAKE_BUILD_TYPE` variable to `Debug`.
2. Press `t` to "toggle".
3. In the `CMAKE_CXX_FLAGS_DEBUG` variable, add the `-pg` option. It will enable the profiling.
4. Configure.
5. Generate.
6. Compile.
7. Submit your job. It will create a file called `gmon.out`.
8. Look at [https://sourceware.org/binutils/docs/gprof/](https://sourceware.org/binutils/docs/gprof/) for information about how to use the profiler.
9. Analyse the results using:

```bash
$ gprof ./main > serial-profiling.txt
$ less serial-profiling.txt
```

10. Identify which step took the longest.


## Enable Pthread in CMake

1. Edit `CMakeLists.txt` and at the end of the file add:

```cmake
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
```

It looks for the `pthread` library.

2. Add the program:

```cmake
add_executable(main-pthreads src/main-pthreads.cxx)
TARGET_LINK_LIBRARIES (main-pthreads PUBLIC RayTracing ${ASSIMP_LIBRARY} Threads::Threads)
```

## Parallelise this step

1. You need to edit `main-pthreads.cxx`. I would copy the content of `main.cxx` into `main-pthreads.cxx`.
2. Work load allocation:
    - Define a structure to store data required by each thread. You can see an example at Lines 9-15 in [chapter4-3-2.cxx](../Lecture-4/chapter4-3-2.cxx). **In your case, the polygon meshes, the light, maybe some other parameters, and the output image have to be accessible from the thread.**
    - Compute the number of pixels processed by each thread. You can see an example at Lines 45-66 in [chapter4-3-2.cxx](../Lecture-4/chapter4-3-2.cxx) for an example.
3. Create *T* threads:
    - Use the `pthread_create` function.
    - See the slides from Chapter 4 for an explanation, and
    - See Lines 18, 71-78, & 117-121 in [chapter4-3-3.cxx](../Lecture-4/chapter4-3-3.cxx) for an example.
4. Wait for all the threads to complete before saving the output image into a file.
    - Use the `pthread_join` function.
    - See the slides from Chapter 4 for an explanation, and
    - See Lines 80-84 in [chapter4-3-4.cxx](../Lecture-4/chapter4-3-4.cxx) for an example.
5. Write the callback
    - The lines of code corresponding to the most computationally intensive task has to be ported to the callback function.
    - See Lines 15-16, 34-36, 51, 63-64, 91-96, & 127-142 in [chapter4-3-5.cxx](../Lecture-4/chapter4-3-5.cxx) for an example.
7. Create a new script to submit the parallel job, e.g. `submit-pthread.sh`:

```bash
#!/usr/bin/env bash
#

#SBATCH --job-name=my_test # Job name
#SBATCH --nodes=1                    # Use one node
#SBATCH --mem=600mb                  # Total memory limit
#SBATCH --time=01:00:00              # Time limit hrs:min:sec

./main-pthreads --jpeg test.jpg -b 128 0 0 -s 512 256 -t $SLURM_CPUS_PER_TASK
```
7. Submit your job:

```bash
$ sbatch  --account=scw1563 -c N submit-pthread.sh
```
**Note: replace N above with a number between 1 and 40.**

- Check that the output image is correct. Is it the same as the image generated with the serial code?
- See if you have any speedup when you increase the number of thread.

## Generate a spreadsheet

- Create your own spreadsheet in a `.csv` format. See [chap4-performance.csv](../Lecture-4/chap4-performance.csv) for an example.
- Run gnuplot to generate graphs showing the execution time depending on the number of threads, and the speedup depending on the number of threads. See [chap4-performance.plt](../Lecture-4/chap4-performance.plt) for an example. This example generates PNG files such as:

![Plot of the execution time](../Lecture-4/chap4-performance-1.png)
![Plot of the execution time](../Lecture-4/chap4-performance-2.png)

- For the ray tracing problem, I provide two scripts, one for the [serial code](https://github.com/effepivi/SimpleRayTracing/blob/master/submit-serial.sh), one for the [pthread code](https://github.com/effepivi/SimpleRayTracing/blob/master/submit-pthread.sh).
- Edit this line `##SBATCH --mail-user=YOUREMAILADDRESS@bangor.ac.uk  # Where to send mail` to use your own email address. Also remove one of the `#` characters.
- I also provided a new python script to plot the data [`plotTiming.py`](https://github.com/effepivi/SimpleRayTracing/blob/master/plotTiming.py) -->
