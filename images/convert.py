from sys import argv
import cv2 as cv
import numpy as np

# img = open(argv[1], "r")
# print(img.read())

img = cv.imread("c:/Zichen/Cornell/fall2022/CS4999/clover/images/image.ppm")
cv.imwrite("c:/Zichen/Cornell/fall2022/CS4999/clover/images/image.jpg", img)
