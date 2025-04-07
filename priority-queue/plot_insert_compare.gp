set terminal png size 800,600 enhanced font 'Verdana,10'
set output output_file

set title plot_title
set xlabel 'Number of Elements (N)'
set ylabel 'Time (seconds)'
set key top left autotitle columnheader
set grid
set format y '%.2e'

f_lin(x) = c*x + d
f_log(x) = a * log(x) + b

c=0; d=0;
fit f_lin(x) input_file_set using 1:2 via c, d

a=1e-9; b=1e-9;
fit f_log(x) input_file_binary using ( ($1 > 0) ? $1 : 1):2 via a, b

plot \
    input_file_set using 1:2 with linespoints pt 6 ps 0.5 title 'Set-based Insert (O(N) expected)', \
    f_lin(x) with lines lw 2 title sprintf('Fit Set O(N): %.2e*x+%.2e', c, d), \
    input_file_binary using 1:2 with linespoints pt 7 ps 0.5 title 'Binary Heap Insert (O(log N) expected)', \
    f_log(x) with lines lw 2 title sprintf('Fit Heap O(log N): %.2e*log(x)+%.2e', a, b)

unset output
