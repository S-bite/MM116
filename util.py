with open("tmp.txt", "w+") as f:
    for mag in range(0, 30):
        for seed in range(3):
            for n in range(5, 100, 20):
                f.write(f"0.5 {n} {seed} {mag/10}\n")
