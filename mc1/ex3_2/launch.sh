#!/bin/bash

export LC_NUMERIC=C
set -o pipefail

rm -f *.dat
make

run_once() {
    ./mc.x parameters.1 "$1" "$2" | awk 'NR > 1 { trials += $7; step = $1 } END { if (trials > 0) printf "%.0f %.8f\n", trials, step / trials }'
}

# progressive gamma
seed=0
for gam in $(seq 0.05 0.025 0.25); do
    output_file="out.$gam.dat"
    : > "$output_file"

    for run in {1..300}; do
        seed=$((seed + 1))
        if result=$(run_once "$gam" "$seed"); then
            printf '%s\n' "$result" >> "$output_file"
        fi
    done

    if ! awk -v gam="$gam" 'NF == 2 { trials += $1; acceptance += $2; runs++ } END { if (runs != 300) { printf "gamma %s: got %d qualifying runs, expected 300\n", gam, runs > "/dev/stderr"; exit 1 } printf "%s %.6f %.6f\n", gam, trials/runs, acceptance/runs }' "$output_file" >> data.dat; then
        rm -f "$output_file"
        exit 1
    fi

    rm "$output_file"
done

gnuplot plot.gnuplot