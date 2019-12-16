set title "perf"
set xlabel "distance"
set ylabel "cycles"
set terminal png font " Times_New_Roman,12 "
set key left
set output "result.png"

plot \
"data.txt" using 1:2 with linespoints linewidth 2 title "orig", \
"data.txt" using 1:3 with linespoints linewidth 2 title "rawmemchr"