reset
set ylabel 'error rate'
set xlabel 'N'
set title 'compute-pi'
set terminal png font 'Times_New_Roman,12'
set output 'errorrate.png'
set datafile sep ','

plot \
'result_clock_gettime.csv' using 1:2 with lines linewidth 2 title 'baseline', \
'result_clock_gettime.csv' using 1:4 with lines linewidth 2 title 'openmp2', \
'result_clock_gettime.csv' using 1:6 with lines linewidth 2 title 'openmp4', \
'result_clock_gettime.csv' using 1:8 with lines linewidth 2 title 'avx', \
'result_clock_gettime.csv' using 1:10 with lines linewidth 2 title 'avxunroll' \

reset
set ylabel 'time(sec)'
set xlabel 'N'
set title 'compute-pi'
set terminal png font 'Times_New_Roman,12'
set output 'runtime.png'
set datafile sep ','
set logscale x

plot \
'result_clock_gettime.csv' using 1:3 smooth bezier linewidth 2 title 'baseline', \
'result_clock_gettime.csv' using 1:5 smooth bezier linewidth 2 title 'openmp2', \
'result_clock_gettime.csv' using 1:7 smooth bezier linewidth 2 title 'openmp4', \
'result_clock_gettime.csv' using 1:9 smooth bezier linewidth 2 title 'avx', \
'result_clock_gettime.csv' using 1:11 smooth bezier linewidth 2 title 'avxunroll' \
