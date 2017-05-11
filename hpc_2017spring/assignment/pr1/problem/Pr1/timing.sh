#!/bin/csh
echo -n "set term X11; set grid; set xlabel 'Dim'; set ylabel 'MFlop/s'; plot" > timing.gnuplot
set sep =
foreach prog ($*)
  echo "Executing $prog to gather data..."
  (./$prog || echo "0 0") > $prog.data
  echo -n "$sep '$prog.data' with lines title '$prog'" >> timing.gnuplot
  set sep = ,
end
echo "" >> timing.gnuplot
echo "To display the plot, use: gnuplot -persist timing.gnuplot"
