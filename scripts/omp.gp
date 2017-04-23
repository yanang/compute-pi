reset
set ylabel 'runtime(s)'
set xlabel 'N'
set title 'compute-pi'
set terminal png font 'Times_New_Roman,12'
set output 'omp.png'
set datafile sep ','
set xtics 0,70000,500000
plot \
'result_compare_omp_threads.csv' using 1:2 smooth bezier linewidth 2 title 'OMP', \
'result_compare_omp_threads.csv' using 1:3 smooth bezier linewidth 2 title 'OMP-2-thread', \
'result_compare_omp_threads.csv' using 1:4 smooth bezier linewidth 2 title 'OMP-4-thread', \
'result_compare_omp_threads.csv' using 1:5 smooth bezier linewidth 2 title 'OMP-8-thread', \
'result_compare_omp_threads.csv' using 1:6 smooth bezier linewidth 2 title 'OMP-16-thread', \
'result_compare_omp_threads.csv' using 1:7 smooth bezier linewidth 2 title 'OMP-32-thread', \
'result_compare_omp_threads.csv' using 1:8 smooth bezier linewidth 2 title 'OMP-64-thread', \
'result_compare_omp_threads.csv' using 1:9 smooth bezier linewidth 2 title 'OMP-128-thread', \
'result_compare_omp_threads.csv' using 1:10 smooth bezier linewidth 2 title 'OMP-256-thread', \
'result_compare_omp_threads.csv' using 1:11 smooth bezier linewidth 2 title 'OMP-512-thread', \
'result_compare_omp_threads.csv' using 1:12 smooth bezier linewidth 2 title 'OMP-1024-thread', \


