from random import randint

MIN, MAX = -10, 10
N_PONTOS = 6
N_CURVAS = 8

pontos = [(randint(MIN, MAX) for _ in range(2)) for _ in range(N_PONTOS)]

curvas = []
for _ in range(N_CURVAS):
    curva = []

    while len(curva) < 4:
        next = randint(0, N_PONTOS-1)
        if next not in curva:
            curva.append(next)

    curvas.append(curva)

with open("points.txt", "w") as file:
    for p in pontos:
        for i in p:
            file.write(f"{i} ")
        file.write("\n")

with open("curves.txt", "w") as file:
    for c in curvas:
        for i in c:
            file.write(f"{i} ")
        file.write("\n")
