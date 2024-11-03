set terminal pngcairo size 1920,1080
set output 'benchmark_plot.png'
set title 'Porównanie Szybkości Algorytmów Sortowania dla różnych rozmiarów danych'
set ylabel 'Czas Wykonania (ms)'
set xlabel 'Algorytm Sortowania'
set style data histogram
set style histogram cluster gap 1
set style fill solid 0.6
set boxwidth 0.9
set logscale y 
set key outside right top vertical
set xtics rotate by -45

plot 'benchmark_data.dat' using 2:xtic(1) title 'Rozmiar: 10,000', \
     '' using 3 title 'Rozmiar: 25,000', \
     '' using 4 title 'Rozmiar: 50,000', \
     '' using 5 title 'Rozmiar: 100,000'