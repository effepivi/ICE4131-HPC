# ICE4131 - High Performance Computing (HPC)
## Lab 2: How to launch “Jobs” using SLURM
### Tutor: Franck Vidal

## Objectives

<!-- Link to create images of code: https://carbon.now.sh -->

SuperComputing Wales uses SLURM (Simple Linux Utility for Resource Management). It is a free and open-source job scheduler for Linux and Unix-like operating systems. It is mainly used by supercomputers and computer clusters. In today’s lab, you will learn how to use it to submit your work to SuperComputing Wales’ supercomputer.

So far you have executed your programs using `./PROGRAM_NAME`, e.g.:

```bash
$ ./HelloWorld
```

The overall topology of the supercomputer looks like:

![SCW overall topology](../Lab-1/topology.png)

It is not good practice to run your code on the login server when using a supercomputer. It is because the execution will be performed on `hawklogin.cf.ac.uk` rather than a compute node. Absolutely **ALL** the users currently logged in are using `hawklogin.cf.ac.uk` and share its resources. Instead it is better to use [SLURM](https://slurm.schedmd.com/documentation.html) to make
sure the code is running on a dedicated compute node rather than a shared resource. This way you can maximise performance, and you won't bother other users.

## sinfo

`sinfo` lists the partitions that are available. A partition is a set of compute nodes (computers dedicated to ... computing,) grouped logically. Typical examples include partitions dedicated to batch processing, debugging, post processing, or visualisation. Try to run the `sinfo` command. You'll see something like:

```bash
$ sinfo
PARTITION AVAIL  TIMELIMIT  NODES  STATE NODELIST
compute*     up 3-00:00:00      9    mix ccs[0120,0122,0126,0129-0130,0132-0133,1010,1018]
compute*     up 3-00:00:00    141  alloc ccs[0001-0119,0121,0123-0125,0127-0128,0134,1009,1011-1017,1019-1022,1024-1026]
compute*     up 3-00:00:00      2   idle ccs[0131,1023]
highmem      up 3-00:00:00      2    mix ccs[1010,1018]
highmem      up 3-00:00:00     23  alloc ccs[1001-1009,1011-1017,1019-1022,1024-1026]
highmem      up 3-00:00:00      1   idle ccs1023
gpu          up 2-00:00:00      3    mix ccs[2003,2010-2011]
gpu          up 2-00:00:00     10  alloc ccs[2001-2002,2004-2009,2012-2013]
htc          up 3-00:00:00     15    mix ccs[0120,0122,0126,0129-0130,0132-0133,1010,1018,2010-2011,3004,3012,3015,3024]
htc          up 3-00:00:00     48  alloc ccs[0121,0123-0125,0127-0128,0134,1009,1011-1017,1019-1022,1024-1026,2008-2009,2012-2013,3001-3003,3005-3011,3013-3014,3016-3023,3025-3026]
htc          up 3-00:00:00      2   idle ccs[0131,1023]
dev          up    1:00:00      1    mix ccs0135
dev          up    1:00:00      1  alloc ccs0136
```

The command `sinfo` can output the information in a node-oriented fashion, with the argument `-N`. `-l` . Try them:

```bash
$ sinfo -N -l
Thu Oct 17 11:55:05 2019
NODELIST   NODES PARTITION       STATE CPUS    S:C:T MEMORY TMP_DISK WEIGHT AVAIL_FE REASON              
ccs0001        1  compute*   allocated   40   2:20:1 191000        0      1   (null) none                
ccs0002        1  compute*   allocated   40   2:20:1 191000        0      1   (null) none                
ccs0003        1  compute*   allocated   40   2:20:1 191000        0      1   (null) none                
...
...
ccs0118        1  compute*   allocated   40   2:20:1 191000        0      1   (null) none                
ccs0119        1  compute*   allocated   40   2:20:1 191000        0      1   (null) none                
ccs0120        1       htc       mixed   40   2:20:1 191000        0      1   (null) none                
ccs0120        1  compute*       mixed   40   2:20:1 191000        0      1   (null) none                
ccs0121        1       htc   allocated   40   2:20:1 191000        0      1   (null) none                
...
...
ccs1008        1   highmem   allocated   40   2:20:1 383000        0      1   (null) none                
ccs1009        1   highmem   allocated   40   2:20:1 383000        0      1   (null) none                
...
...
ccs2001        1       gpu   allocated   40   2:20:1 383000        0      1   (null) none                
ccs2002        1       gpu   allocated   40   2:20:1 383000        0      1   (null) none                
```

## squeue

The squeue command shows the list of jobs which are currently running (they are in the RUNNING state, noted as ’R’) or waiting for resources (noted as ’PD’). Try it:

```bash
$ squeue
```

You'll see all the current jobs. In most cases, you are only interested in yours. Execute `squeue -u $USER`

```bash
$ squeue -u $USER
JOBID PARTITION     NAME     USER ST       TIME  NODES NODELIST(REASON)
7364890      work     test franck.v PD       0:00      1 (None)
7364888      work     test franck.v  R       0:02      1 bwc053
7364889      work     test franck.v  R       0:02      1 bwc053
$
```

## Creating a job
A job consists in two parts:

1. **Resource requests** consist in a number
of CPUs, computing expected duration, amounts of RAM or disk space, etc.
2. **Job steps** describe the tasks that must be done, software which must be run.

- Create a new directory called `LAB2` using the `mkdir` command.
- Go into that directory using the `cd` command.
- Use `nano` to create a file named `submit.sh`:
```bash
$ nano submit.sh
```
The file should contain:
```bash
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
```
- Before you launch your job, type:
```bash
export SCW_TPN_OVERRIDE=1
```
This is because we are only using 1 thread.
- To launch the job, you need to use ```sbatch``` as follows:
```bash
$ sbatch  --account=scw1563 submit.sh
```
- In the console, you will see the job number, e.g.:
```bash
Submitted batch job 2825285
```
- A file, ```slurm-2825285.out``` will be created. To visualise their content, type:
```bash
more  slurm-2825285.out
```

## Back to the code of Lab 1

- Go into the directory where the programs corresponding to Lab 1 are. Use the `cd` to change directory, and the `ls` command to check the content of the directory.
- Copy the content of `HelloWorld-pthread3.cxx` into `HelloWorld-pthread4.cxx`.
- The user should be able to set the number of thread using the command line arguments.
- Modify the main accordingly. See below for an example:
```c++
int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << "\t" << "N   [with N the number of threads]" << endl;
        return EXIT_FAILURE;
    }

    int N = atoi(argv[1]);

    ...
    ...

    return 0;
}
```
- Compile it using:
```bash
$ g++ HelloWorld-pthread4.cxx -lpthread -o HelloWorld-pthread4
```
- Create a new file named `submit.sh` containing:
```bash
#!/bin/bash
#

#SBATCH --job-name=my_test # Job name
#SBATCH --nodes=1                    # Use one node
#SBATCH --mem=600mb                  # Total memory limit
#SBATCH --time=00:15:00              # Time limit hrs:min:sec

./HelloWorld-pthread4 $SLURM_CPUS_PER_TASK
```

To launch it, use:
```bash
$ sbatch  --account=scw1563 -c N submit.sh
```
**Note: replace N above with a number between 1 and 40.**

We use an environment variable, `SLURM_CPUS_PER_TASK`. It corresponds the number of threads that you want to use.
We requested one computing node with `#SBATCH --nodes=1`, and the maximum number of CPU cores is 40.
Now, test your code with various number of threads and check the contents in the output files.
