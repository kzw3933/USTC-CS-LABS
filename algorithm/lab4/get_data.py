import math
import random

vertex_scale = [27, 81, 243, 729]
weights = [i for i in range(-10, 51, 1)]
lines1 = []
lines2 = []

for index, i in enumerate(vertex_scale):
    lines1.clear()
    lines2.clear()
    vertexs = [i for i in range(i)]
    degree1 = int(math.log(i, 5))
    degree2 = int(math.log(i, 7))

    for j in range(i):
        vertex_list = []
        for k in range(degree1):
            vertex = random.choice(vertexs)
            while vertex == j or vertex in vertex_list:
                vertex = random.choice(vertexs)
            vertex_list.append(vertex)
            weight = random.choice(weights)
            lines1.append([str(j), str(vertex), str(weight)])

        with open("E:\\Savefiles\\Labs\\Algorithm\\lab4\\ex1\\input\\input" + str(index + 1) + "1.txt", "w") as f:
            for line in lines1:
                f.write("\t".join(line) + "\n")

        vertex_list = []
        for k in range(degree2):
            vertex = random.choice(vertexs)
            while vertex == j or vertex in vertex_list:
                vertex = random.choice(vertexs)
            vertex_list.append(vertex)
            weight = random.choice(weights)
            lines2.append([str(j), str(vertex), str(weight)])

        with open("E:\\Savefiles\\Labs\\Algorithm\\lab4\\ex1\\input\\input" + str(index + 1) + "2.txt", "w") as f:
            for line in lines2:
                f.write("\t".join(line) + "\n")

