set fit logfile 'figures/fitlogs/4.log' quiet
f(x) = 400*exp(-(x - A)/B)
A=5000
B=500000
fit f(x) "data/4.txt" via A, B

set terminal pdf enhanced font 'Verdana,12'
set output "figures/4.pdf"

set xlabel 'Time'
set ylabel 'Molecles in container'

plot "data/4.txt" pt 7 lc 'red' lw 2, f(x) lw 3 lc 'black'