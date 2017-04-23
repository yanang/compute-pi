reset
set ylabel 'time(sec)'
set style fill solid
set title 'compute-pi'
set term png enhanced font 'Verdana,10'
set output 'omp_hi.png'
set datafile sep ','

plot [:][:]\
'result_compare_omp_simd_threads.csv' using 2:xtic(1) with histogram title 'OMP-mutlithread', \


  
