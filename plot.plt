#/usr/bin/env gnuplot

set terminal png
set xlabel "Thermal energy"
set grid

set output "energy.png"
set ylabel "E"
plot "data_all" using 1:2 with linespoints

set output "magnetization.png"
set ylabel "M/M0"
plot "data_all" using 1:4 with linespoints

set output "energy_errors.png"
set ylabel "E"
plot "data_all" using 1:2:3 with errorbars

set output "magnetization_errors.png"
set ylabel "M/M0"
plot "data_all" using 1:4:5 with errorbars

set output "susceptibility.png"
set ylabel "chi"
plot "data_all" using 1:($5 ** 2) with points

set output "specific_heat.png"
set ylabel "c"
plot "data_all" using 1:($3 ** 2) with points
