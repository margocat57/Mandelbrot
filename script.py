import sys
import csv

# script.py <target> <compiler> <opt>

target, compiler, opt = sys.argv[1], sys.argv[2], sys.argv[3]

lines = sys.stdin.read().splitlines()

with open("results.csv", "a", newline="") as f:
    writer = csv.writer(f, delimiter=';')
    for line in lines:
        try:
            time_val = float(line.strip())
            writer.writerow([target, compiler, opt, time_val])
        except ValueError:
            continue
