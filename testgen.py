import argparse
import random


def printGrid():
    h = random.randint(2, 10)
    w = random.randint(2, 10)
    print(h)
    for i in range(h):
        line = ""
        for j in range(w):
            line += chr(ord('A')+random.randint(0, 25))
        print(line)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='Generate Testcase for MM116')
    parser.add_argument('seed')
    parser.add_argument('-n', default=-1)
    parser.add_argument('-p', default=-1)
    args = parser.parse_args()
    seed = args.seed
    random.seed(seed)
    n = args.n
    if(n == -1):
        n = random.randint(5, 100)
    p = args.p
    if(p == -1):
        p = random.random()*0.9+0.05
    print(p, n)
    for i in range(n):
        printGrid()
