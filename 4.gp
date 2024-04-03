set fit logfile 'figures/fitlogs/4.log' quiet
f(x) = 400*exp(-(x - A)/B)
A=5000
B=500000
fit f(x) "data/4.txt" via A, B

set terminal pdf enhanced font 'Verdana,12'
set output "figures/4.pdf"

# set logscale x
set xlabel 'Time'   
set xtics rotate by 45 right
set ylabel 'Molecles in container'
set title 'Molecules in container over time'

plot "data/4.txt" pt 7 lc 'red' lw 2, f(x) lw 3 lc 'black'