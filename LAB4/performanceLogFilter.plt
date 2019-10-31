set title "Log filter on REPLACE WITH CPU"

set xlabel "Number of threads/processes"

set datafile separator ","

set term png

f(x) = x



serial_excution=1.24429




set ylabel "Execution time in sec"
set output "log_execution_time.png"

plot "log-pthread.csv" using 5:6 w l title "Phtreads", \
     "log-openmp.csv"  using 5:6 w l title "OpenMP"


set ylabel "Speedup factor"
set output "log_speedup.png"

plot f(x) with l title "Theoretical", \
    "log-pthread.csv" using 5:(serial_excution / $6) w l title "Phtreads", \
    "log-openmp.csv"  using 5:(serial_excution / $6) w l title "OpenMP"
