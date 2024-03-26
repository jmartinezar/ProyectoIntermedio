f(x) = a*x**2 + b*x
#f(x) = c*x**2
fit f(x) "2.txt" via a, b

set key left top
set terminal pdf enhanced font 'Verdana,12'
set output '2.pdf'

set xlabel 'size'
set ylabel 't_{eq}'
set title 'Time of equilibrium versus size'

plot '2.txt' with points t "Data" pt 7 lc 'red' lw 2.5, f(x) t "Quadratic fit" lw 3 lc 'black'
