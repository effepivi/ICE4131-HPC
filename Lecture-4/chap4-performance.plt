set title "Performance on AMD® Fx(tm)-8350 eight-core processor × 8"
set xlabel "Number of threads"
set ylabel "Execution time in sec"

set term png
set output "chap4-performance-1.png"
plot "chap4-performance.csv" using 1:2 w l notitle

set yrange[0:20]
set ylabel "Speedup factor"
set output "chap4-performance-2.png"
f(x) = x
plot f(x) with l title "Linear speedup", "chap4-performance.csv" using 1:3 w l title "Actual speedup"
