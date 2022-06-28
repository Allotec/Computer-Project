file = open("instructions.txt", "w")

with open('dump.txt') as f:
    lines = f.readlines()

for line in lines:
    splitLine = line.split()
    file.write(splitLine[1] + "\n")