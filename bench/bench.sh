#!/usr/bin/env bash

export TIMEFORMAT=%U
cd ops

for op in *; do
    case $op in
        macro) continue;;
        f* | div) count=1000000;;
        *) count=100000000;;
    esac
    cd $op
    fasm baseline.asm -d count=$count > /dev/null
    fasm bench.asm -d count=$count > /dev/null
    bench=$( { time ./bench; } 2>&1 )
    baseline=$( { time ./baseline; } 2>&1 )
    score=$(echo "x=$count/($bench-$baseline); if (x<1) print 0; x" | bc)
    printf '%s\t%s\n' "$op" "$score"
    cd ..
done
