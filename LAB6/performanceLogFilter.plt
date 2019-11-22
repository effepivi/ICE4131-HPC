
set xlabel "Number of threads/processes"

set datafile separator ","

set term png

f(x) = x


set ylabel "Execution time in sec"
set output "log_execution_time.png"

plot "../LAB4/log-pthread.csv" using 5:6 w l title "Phtreads", \
     "../LAB4/log-openmp.csv"  using 5:6 w l title "OpenMP", \
     "../LAB5/log-MPI.csv"  using 5:6 w l title "MPI", \
     "../LAB6/log-CUDA.csv"  using 5:6 w l title "CUDA"


set ylabel "Speedup factor"
set output "log_speedup.png"
set yrange[0:10]

plot f(x) with l title "Theoretical", \
    "../LAB4/log-pthread.csv" using 5:(serial_excution / $6) w l title "Phtreads", \
    "../LAB4/log-openmp.csv"  using 5:(serial_excution / $6) w l title "OpenMP", \
    "../LAB5/log-MPI.csv"     using 5:(serial_excution / $6) w l title "MPI", \
    "../LAB6/log-CUDA.csv"    using 5:(serial_excution / $6) w l title "CUDA"
