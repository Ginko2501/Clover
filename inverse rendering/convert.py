from sys import argv
import cv2 as cv
import numpy as np

# img = open(argv[1], "r")
# print(img.read())

# # Ground Truth image
# img = cv.imread("./GT.ppm")
# cv.imwrite("./GT.jpg", img)

# currenet image 
img = cv.imread("c:/Zichen/Cornell/fall2022/CS4999/clover/inverse rendering/I.ppm")
cv.imwrite("c:/Zichen/Cornell/fall2022/CS4999/clover/inverse rendering/I.jpg", img)
