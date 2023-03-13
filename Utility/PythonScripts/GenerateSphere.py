from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt
import math
from matplotlib.tri import Triangulation

fig = plt.figure()

widthSubdivisions = 10
heightSubdivisions = 10
positionX = 0
positionY = 0
positionZ = 0

radius = 1.0





num_points = widthSubdivisions *  heightSubdivisions
widthStep = 2.0 * math.pi / float(widthSubdivisions - 1)
heightStep =  math.pi / float(heightSubdivisions - 1)

pointsX = []
pointsY = []
pointsZ = []

for i in range(0, heightSubdivisions):
    phi = i * heightStep
    sinPhi = math.sin(phi)
    cosPhi = math.cos(phi)

    for j in range(0, widthSubdivisions):
        theta = j * widthStep
        sinTheta = math.sin(theta)
        cosTheta = math.cos(theta)

        index = (i * widthSubdivisions + j) * 3
        pointsX.append(radius * sinPhi * cosTheta + positionX)
        pointsY.append(radius * sinPhi * sinTheta + positionY)
        pointsZ.append(radius * cosPhi + positionZ)


X = np.array(pointsX)
Y = np.array(pointsY)
Z = np.array(pointsZ)
ax = plt.axes(projection='3d')
ax.plot_trisurf(X, Y, Z, cmap="jet", linewidth=0)


plt.show()