from sys import argv
import cv2 as cv
import numpy as np

# img = open(argv[1], "r")
# print(img.read())

img = cv.imread("image.ppm")
cv.imwrite("./image.jpg", img)
