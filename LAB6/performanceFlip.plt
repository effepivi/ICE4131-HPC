
set xlabel "Number of threads/processes"

set datafile separator ","

set term png

f(x) = x
g(x) = cuda_excution
h(x) = serial_excution / cuda_excution

set ylabel "Execution time in sec"
set output "flip_execution_time.png"

plot "../LAB4/flip-pthread.csv" using 5:6 w l title "Phtreads", \
     "../LAB4/flip-openmp.csv"  using 5:6 w l title "OpenMP", \
     "../LAB5/flip-MPI.csv"  using 5:6 w l title "MPI", \
     g(x) w l title "CUDA"


set ylabel "Speedup factor"
set output "flip_speedup.png"
set yrange[0:10]

plot f(x) with l title "Theoretical", \
    "../LAB4/flip-pthread.csv" using 5:(serial_excution / $6) w l title "Phtreads", \
    "../LAB4/flip-openmp.csv"  using 5:(serial_excution / $6) w l title "OpenMP", \
    "../LAB5/flip-MPI.csv"     using 5:(serial_excution / $6) w l title "MPI", \
    h(x) w l title "CUDA"
