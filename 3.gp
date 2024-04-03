stats 'data/3.txt' nooutput
N = STATS_records
# print(N)
stats "data/3.txt" every ::N-1::N-1 u 2 nooutput
t_fill = int(STATS_max)
# print(t_fill)

set fit logfile 'figures/fitlogs/3.log' quiet
f(x) = a*sqrt(x + b) + c
fit[:t_fill] f(x) "data/3.txt" via a, b, c

set key right bottom
set terminal pdf enhanced font 'Verdana,12'
set output 'figures/3.pdf'

set yrange [0:250]

set label 1 'filling time of the drop' at t_fill + 20000, 100
set xlabel 'time'
set ylabel 'size of the drop'
set title 'Size of the drop as a function of time'

set arrow from t_fill,0 to t_fill,250 nohead lc 2 lw 2
plot 'data/3.txt' every ::::N-2 w l t 'Data', f(x) lw 1.5 lc 4 t 'Square root fit'
# plot '3.txt' using 1:2 w lp t 'Data', f(x) t 'Square root fit'