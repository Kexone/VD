set datafile separator ","
set xlabel "xxx" rotate parallel
set ylabel "yyy" rotate parallel
set zlabel "zzz" rotate parallel
set xrange [-3:3]
set yrange [-3:3]
set term wxt 0
splot 'gauss.txt' using 1:2:3 with points palette pointsize 3 pointtype 7