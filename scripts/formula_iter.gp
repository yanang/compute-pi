reset
set ylabel 'N(times)'
set xlabel 'M(error rate < 10^{-M})'
set title 'compute-pi'
set terminal png font 'Times_New_Roman,12'
set output 'formula_iter.png'
set datafile sep ','

plot \
'result_formula_iter_times.csv' using 1:2 with linespoints linewidth 2 title 'Riemann', \
'result_formula_iter_times.csv' using 1:3 with linespoints linewidth 2 title 'Lebiniz', \
'result_formula_iter_times.csv' using 1:4 with linespoints linewidth 2 title 'Wallis', \
'result_formula_iter_times.csv' using 1:5 with linespoints linewidth 2 title 'Nilakantha', \
'result_formula_iter_times.csv' using 1:6 with linespoints linewidth 2 title 'Chudnovsky', \


