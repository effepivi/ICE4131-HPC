#!/usr/bin/env bash

# Change the input image path if needed
INPUT_IMAGE="./Airbus_Pleiades_50cm_8bit_grey_Yogyakarta.txt"

# Header for the CSV files
header="\"input_file\",implementation,number_of_processes_or_threads,duration_in_sec"

echo "Log_filter,"$header  > log-openmp.csv
echo "Log_filter,"$header  > log-pthread.csv
echo "Flip_filter,"$header > flip-openmp.csv
echo "Flip_filter,"$header > flip-pthread.csv

# Get the CPU name
CPU=`lscpu | sed -nr '/Model name/ s/.*:\s*(.*) @ .*/\1/p'`

# Run the log filter with the serial implementation
TEMP=`./bin/log -c serial -i $INPUT_IMAGE`
IFS=',' read -ra LOG_SERIAL <<< $TEMP

# Store the results in the corresponding CSV files
echo $TEMP >> log-openmp.csv
echo $TEMP >> log-pthread.csv

# Create the corresponding gnuplot script
# Set the title using the CPU name
echo "set title 'Log filter on $CPU'" > temp_log.plt
# Insert the runtime of the serial implementation
echo "serial_excution=${LOG_SERIAL[-1]}" >> temp_log.plt
cat performanceLogFilter.plt >> temp_log.plt


# Run the flip the image with the serial implementation
TEMP=`./bin/flip -H -c serial -i $INPUT_IMAGE`
IFS=',' read -ra FLIP_SERIAL <<< $TEMP

# Store the results in the corresponding CSV files
echo $TEMP >> flip-openmp.csv
echo $TEMP >> flip-pthread.csv

# Create the corresponding gnuplot script
# Set the title using the CPU name
echo "set title 'Flip image on $CPU'" > temp_flip.plt
# Insert the runtime of the serial implementation
echo "serial_excution=${FLIP_SERIAL[-1]}" >> temp_flip.plt
cat performanceFlip.plt >> temp_flip.plt


# Run the program using OpenMP and Phtread with the number of threads
# from 1 to 40
for i in {1..40}
do
    ./bin/log     -c pthread -i $INPUT_IMAGE -n $i >> log-pthread.csv
    ./bin/log     -c openmp  -i $INPUT_IMAGE -n $i >> log-openmp.csv

    ./bin/flip -H -c pthread -i $INPUT_IMAGE -n $i >> flip-pthread.csv
    ./bin/flip -H -c openmp  -i $INPUT_IMAGE -n $i >> flip-openmp.csv
done

# Remove old graphs
rm -f  log_execution_time.png  log_speedup.png
rm -f flip_execution_time.png flip_speedup.png

# Create graphs with gnuplot
gnuplot < temp_log.plt
gnuplot < temp_flip.plt
