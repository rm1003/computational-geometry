#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
from matplotlib.pyplot import figure, show

# Listas globais
abscissas = []
ordenadas = []
px = []
py = []

# Limites do desenho
maxx = -99999
maxy = -99999
minx = 99999
miny = 99999

def le_poligono():
    global maxx, maxy, minx, miny

    ax = []
    ay = []
    n = int(sys.stdin.readline())
    for _ in range(n):
        a = sys.stdin.readline().split()
        x = int(a[0])
        y = int(a[1])
        ax.append(x)
        ay.append(y)

        maxx = max(maxx, x)
        minx = min(minx, x)
        maxy = max(maxy, y)
        miny = min(miny, y)

    # Fechar o polígono
    ax.append(ax[0])
    ay.append(ay[0])

    abscissas.append(ax)
    ordenadas.append(ay)

def le_entrada():
    global maxx, maxy, minx, miny

    a = sys.stdin.readline().split()
    n_poly = int(a[0])
    n_point = int(a[1])

    for _ in range(n_poly):
        le_poligono()

    for _ in range(n_point):
        a = sys.stdin.readline().split()
        x = int(a[0])
        y = int(a[1])

        maxx = max(maxx, x)
        minx = min(minx, x)
        maxy = max(maxy, y)
        miny = min(miny, y)

        px.append(x)
        py.append(y)

    return n_poly, n_point

def main():
    global maxx, maxy, minx, miny

    n_poly, n_point = le_entrada()

    dx = maxx - minx
    dy = maxy - miny
    ex = dx * 0.1
    ey = dy * 0.1
    e = int(max(ex, ey)) + 1

    # Aumentar o tamanho da figura para bordas
    dx += 2 * e
    dy += 2 * e
    maxx += e
    maxy += e
    minx -= e
    miny -= e

    fig = figure(1, figsize=(dx, dy))
    ax = fig.add_subplot(111, autoscale_on=False, xlim=(minx, maxx), ylim=(miny, maxy))

    # Desenha os polígonos
    for p in range(n_poly):
        ax.plot(abscissas[p], ordenadas[p], lw=3, color='black')

    # Desenha os pontos
    for p in range(n_point):
        ax.plot(px[p], py[p], 'ro')

    show()

if __name__ == "__main__":
    main()

