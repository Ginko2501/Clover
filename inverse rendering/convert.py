from sys import argv
import cv2 as cv
import numpy as np

# img = open(argv[1], "r")
# print(img.read())

# Ground Truth image
img = cv.imread("./GT.ppm")
cv.imwrite("./GT.jpg", img)

# currenet image 
img = cv.imread("./I.ppm")
cv.imwrite("./I.jpg", img)
