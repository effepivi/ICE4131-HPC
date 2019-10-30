#!/usr/bin/env bash

: > log-pthread.csv
: > log-openmp.csv

header="\"input_file\",implementation,number_of_processes_or_threads,duration_in_sec"

echo "Log_filter,"$header > log-openmp.csv
echo "Log_filter,"$header > log-pthread.csv
echo "Flip_filter,"$header > flip-openmp.csv
echo "Flip_filter,"$header > flip-pthread.csv

for i in {0..12}
do
    ./log -c pthread -i ../Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt -n $i >> log-pthread.csv
    ./log -c openmp  -i ../Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt -n $i >> log-openmp.csv

    ./flip -H -c pthread -i ../Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt -n $i >> flip-pthread.csv
    ./flip -H -c openmp  -i ../Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt -n $i >> flip-openmp.csv

done

gnuplot < performanceLogFilter.plt 
gnuplot < performanceFlip.plt
