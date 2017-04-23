reset
set ylabel 'runtime(s)'
set xlabel 'N'
set title 'compute-pi'
set terminal png font 'Times_New_Roman,12'
set output 'runtime.png'
set datafile sep ','
set logscale x

plot \
'result_clock_gettime.csv' using 1:2 smooth bezier linewidth 2 title 'Baseline', \
'result_clock_gettime.csv' using 1:3 smooth bezier linewidth 2 title 'OpenMP-2-thread', \
'result_clock_gettime.csv' using 1:4 smooth bezier linewidth 2 title 'OpenMP-4-thread', \
'result_clock_gettime.csv' using 1:5 smooth bezier linewidth 2 title 'OpenMP-SIMD', \
'result_clock_gettime.csv' using 1:6 smooth bezier linewidth 2 title 'OpenMP-SIMD-2-thread', \
'result_clock_gettime.csv' using 1:7 smooth bezier linewidth 2 title 'OpenMP-SIMD-4-thread', \
'result_clock_gettime.csv' using 1:8 smooth bezier linewidth 2 title 'AVX-SIMD', \
'result_clock_gettime.csv' using 1:9 smooth bezier linewidth 2 title 'AVX-SIMD-unroll', \
'result_clock_gettime.csv' using 1:10 smooth bezier linewidth 2 title 'OpenCL' \


reset
set ylabel 'runtime(s)'
set xlabel 'N'
set title 'compute-pi'
set terminal png font 'Times_New_Roman,12'
set output 'runtime_compare.png'
set datafile sep ','
set logscale x

plot \
'result_clock_gettime.csv' using 1:2 smooth bezier linewidth 2 title 'Baseline', \
'result_clock_gettime.csv' using 1:4 smooth bezier linewidth 2 title 'OpenMP-4-thread', \
'result_clock_gettime.csv' using 1:7 smooth bezier linewidth 2 title 'OpenMP-SIMD-4-thread', \
'result_clock_gettime.csv' using 1:8 smooth bezier linewidth 2 title 'AVX-SIMD', \
'result_clock_gettime.csv' using 1:9 smooth bezier linewidth 2 title 'AVX-SIMD-unroll', \
'result_clock_gettime.csv' using 1:10 smooth bezier linewidth 2 title 'OpenCL' \

