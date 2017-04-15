reset
set ylabel 'error rate'
set xlabel 'N'
set title 'compute-pi'
set terminal png font 'Times_New_Roman,12'
set output 'errorrate.png'
set datafile sep ','

plot \
'result_clock_gettime.csv' using 1:2 with linespoints linewidth 2 title 'baseline', \
'result_clock_gettime.csv' using 1:4 with linespoints linewidth 2 title 'openmp2', \
'result_clock_gettime.csv' using 1:6 with linespoints linewidth 2 title 'openmp4', \
'result_clock_gettime.csv' using 1:8 with linespoints linewidth 2 title 'avx', \
'result_clock_gettime.csv' using 1:10 with linespoints linewidth 2 title 'avxunroll' \

reset
set ylabel 'time(sec)'
set xlabel 'N'
set title 'compute-pi'
set terminal png font 'Times_New_Roman,12'
set output 'runtime.png'
set datafile sep ','

plot \
'result_clock_gettime.csv' using 1:3 with linespoints linewidth 2 title 'baseline', \
'result_clock_gettime.csv' using 1:5 with linespoints linewidth 2 title 'openmp2', \
'result_clock_gettime.csv' using 1:7 with linespoints linewidth 2 title 'openmp4', \
'result_clock_gettime.csv' using 1:9 with linespoints linewidth 2 title 'avx', \
'result_clock_gettime.csv' using 1:11 with linespoints linewidth 2 title 'avxunroll' \
