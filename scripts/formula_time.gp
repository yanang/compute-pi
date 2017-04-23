reset
set ylabel 'runtime(s)'
set xlabel 'M(error rate < 10^{-M})'
set title 'compute-pi'
set terminal png font 'Times_New_Roman,12'
set output 'formula_time.png'
set datafile sep ','

plot \
'result_formula_time.csv' using 1:2 smooth bezier linewidth 2 title 'Riemann', \
'result_formula_time.csv' using 1:3 smooth bezier linewidth 2 title 'Lebiniz', \
'result_formula_time.csv' using 1:4 smooth bezier linewidth 2 title 'Wallis', \
'result_formula_time.csv' using 1:5 smooth bezier linewidth 2 title 'Nilakantha', \
'result_formula_time.csv' using 1:6 smooth bezier linewidth 2 title 'Chudnovsky', \


