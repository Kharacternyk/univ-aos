#!/usr/bin/env bash

gnuplot -e "
set terminal dumb size 135, 40;
set style data histogram;
set yrange[0:];
$1;
plot '-' using 2:xtic(1) notitle
"
