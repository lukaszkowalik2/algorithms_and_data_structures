
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

plot_command = sprintf("plot '%s' using 1:2 with linespoints pt 7 ps 0.5 title 'Data'", input_file)

if (strstrt(pq_type, "pq_set")) {
    c=0; d=0;
    fit f_lin(x) input_file using 1:2 via c, d
    plot_command = sprintf("plot '%s' using 1:2 with linespoints pt 7 ps 0.5 title 'Data (O(N) expected)', \
                                f_lin(x) with lines lw 2 title sprintf('Fit O(N): f(x) = %.2e*x + %.2e', c, d)", input_file)

} else if (strstrt(pq_type, "pq_binary")) {
    a=1e-9; b=1e-9;
    fit f_log(x) input_file using ( ($1 > 0) ? $1 : 1):2 via a, b
    plot_command = sprintf("plot '%s' using 1:2 with linespoints pt 7 ps 0.5 title 'Data (O(log N) expected)', \
                                f_log(x) with lines lw 2 title sprintf('Fit O(log N): f(x) = %.2e*log(x) + %.2e', a, b)", input_file)
}

eval(plot_command)

unset output
