set log x
set terminal pdf enhanced font 'Verdana,12'
set output 'figures/1.pdf'

unset key

set xlabel 'time'
set ylabel 'Entropy'
set title 'Entropy versus time'

plot 'data/1.txt' with lp
