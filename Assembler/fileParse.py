file1 = open("output.txt", "r")
file2 = open("instructions.txt", "r")

flag = 0
lines1 = file1.readlines()
lines2 = file2.readlines()

for i in range(len(lines2)):
    lines1[i] = lines1[i].strip()
    lines2[i] = lines2[i].strip()

    if(lines1[i] != lines2[i]):
        print("Line- " + str(i + 2) + " Output- " + lines1[i] + " Actual- " + lines2[i])
        flag = 1

if flag == 0:
    print("The files contain the same machine code")