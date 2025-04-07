set terminal png size 800,600 enhanced font 'Verdana,10'
set output output_file

set title plot_title
set xlabel xlabel_text
set ylabel 'Time (seconds)'
set key top left autotitle columnheader
set grid
set format y '%.2e'

f_const(x) = a
f_lin(x) = b*x + c

ts_num = (exists("table_size") ? int(table_size) : -1) 
ts_threshold = 100

plot_command = sprintf("plot '%s' using 1:2 with linespoints pt 7 ps 0.5 title 'Data'", input_file)

if (strstrt(operation, "insert") || strstrt(operation, "contains") || strstrt(operation, "remove")) {
    if (ts_num <= ts_threshold && ts_num != -1) {
        b=0; c=0;
        fit f_lin(x) input_file using 1:2 via b, c
        plot_command = sprintf("plot '%s' using 1:2 with linespoints pt 7 ps 0.5 title 'Data (O(N) expected)', \
                                    f_lin(x) with lines lw 2 title sprintf('Fit O(N): f(x) = %.2e*x + %.2e', b, c)", input_file)
    } else {
        fit f_const(x) input_file using 1:2 via a
        plot_command = sprintf("plot '%s' using 1:2 with linespoints pt 7 ps 0.5 title 'Data (O(1) expected)', \
                                    f_const(x) with lines lw 2 title sprintf('Fit O(1): f(x) = %.2e', a)", input_file)
    }
} else if (strstrt(operation, "union") || strstrt(operation, "intersection") || strstrt(operation, "difference") || strstrt(operation, "equals")) {
    b=0; c=0;
    fit f_lin(x) input_file using 1:2 via b, c
    plot_command = sprintf("plot '%s' using 1:2 with linespoints pt 7 ps 0.5 title 'Data (O(N) expected)', \
                                f_lin(x) with lines lw 2 title sprintf('Fit O(N): f(x) = %.2e*x + %.2e', b, c)", input_file)
} else if (strstrt(operation, "map_")) {
    b=0; c=0;
    fit f_lin(x) input_file using 1:2 via b, c
    plot_command = sprintf("plot '%s' using 1:2 with linespoints pt 7 ps 0.5 title 'Data', \
                                f_lin(x) with lines lw 2 title sprintf('Fit (Linear vs Calls): f(x) = %.2e*x + %.2e', b, c)", input_file)

} else {

}

eval(plot_command)

unset output
