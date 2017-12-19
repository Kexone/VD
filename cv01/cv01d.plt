set terminal pdfcairo enhanced color font "sans,6" fontscale 1.0 linewidth 1 rounded dashlength 1 background "white" size 11cm,7cm 
set encoding utf8

set datafile separator ","
set xdata time
set timefmt "%Y.%m"
set format x "%Y.%m"

set output "cv_01d.pdf"

#definovani barvy os
set style line 1 lc rgb '#FF0000' lt 1 lw 3 pt 2 ps 1 
set style line 2 lc rgb '#00FF00' lt 1 lw 3 pt 2 ps 1 
set border 100 back linestyle 80

#mrizka
set style line 81 lt 0
set style line 81 lt rgb "0x00D3D3D3"
set style line 81 lw 0.5
set grid back linestyle 81

set xtics nomirror
set ytics nomirror

set title "Temperature Measurement Over Several Years in Dublin" font "sans-Bold"
set xlabel "Time [month]"
set ylabel "Avg. temp. [°C]"
#set key bottom left Left title "Probes:" enhanced font "sans-Italic" #reverse box
set yrange [-10:25]
set xrange ["1955.02":"2002.02"]
set style line 1 lt rgb "#A00000" lw 2 pt 1

filename="dublin.csv"

f(x) = a * x + b
fit f(x) filename u 1:2 via a,b

plot filename using 1:2 title "Temps" with lines ls 1 ,\
f(x) with lines ls 2 title "Trend"