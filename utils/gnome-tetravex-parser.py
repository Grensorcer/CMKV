#!/usr/bin/env python
import subprocess
from argparse import ArgumentParser
from pathlib import Path


def parse_board(s, size):
    gap = 6
    skip = 6 * size + 3
    tiles = []
    lines = s.split("\n")[1:-2]
    for i in range(0, len(lines), 3):
        trio = lines[i : i + 3]
        l1, l2, l3 = map(lambda s: s[skip:], trio)

        for j in range(size):
            tiles.append(
                l1[2 + j * gap] + l2[j * gap] + l2[4 + j * gap] + l3[2 + j * gap] + "\n"
            )

    return tiles


if __name__ == "__main__":
    parser = ArgumentParser(
        description="Create a file for a tetravex board of a certain size"
    )
    parser.add_argument("size", metavar="S", type=int, help="Size of the board")
    parser.add_argument("number", metavar="N", type=int, help="Number of boards")
    parser.add_argument("outpath", type=str)
    args = parser.parse_args()

    assert args.size >= 3 and args.size <= 6

    for i in range(1, args.number + 1):
        result = subprocess.run(
            ["gnome-tetravex-cli", "new", "--size", str(args.size)],
            capture_output=True,
            encoding="utf-8",
        )

        tiles = parse_board(result.stdout, args.size)

        outpath = Path(args.outpath) / f"gen_{args.size}_{i}.in"
        with open(outpath, "w") as outfile:
            outfile.writelines(tiles)
