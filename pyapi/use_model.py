import cv2
import numpy as np

def check(a):
    image = a
    print(image.shape)
    print(type(image))
    cv2.imshow("kal", image)