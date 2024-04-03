set terminal pdf enhanced font 'Verdana,12'
set output 'figures/exectime.pdf'

set xlabel 'cup size'
set ylabel 'execution time [s]'
set title 'Execution time vs cup size'

plot 'exectime.txt' u 1:2 w lp lc 4 lw 1.5 lt 8 t 'Compiling with -O3', 'exectime.txt' u 1:3 w lp lc 1 lw 1.5 t 'Compiling without -O3'