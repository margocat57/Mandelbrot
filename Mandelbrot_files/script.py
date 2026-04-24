import sys
import csv

# script.py <target> <compiler> <opt> <mode>

target, compiler, opt, mode = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]

lines = sys.stdin.read().splitlines()

with open("results.csv", "a", newline="") as f:
    writer = csv.writer(f, delimiter=';')
    for line in lines:
        try:
            time_val = float(line.strip())
            writer.writerow([target, compiler, opt, mode, time_val])
        except ValueError:
            continue
