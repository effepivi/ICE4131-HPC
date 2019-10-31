
set xlabel "Number of threads/processes"

set datafile separator ","

set term png

f(x) = x


set ylabel "Execution time in sec"
set output "flip_execution_time.png"

plot "flip-pthread.csv" using 5:6 w l title "Phtreads", \
     "flip-openmp.csv"  using 5:6 w l title "OpenMP"


set ylabel "Speedup factor"
set output "flip_speedup.png"

plot f(x) with l title "Theoretical", \
    "flip-pthread.csv" using 5:(serial_excution / $6) w l title "Phtreads", \
    "flip-openmp.csv"  using 5:(serial_excution / $6) w l title "OpenMP"
