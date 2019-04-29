from keras import models as km
import cv2
from os import environ
import numpy as np

# TODO: order the classes correctly

catagories = ["airplane", "automobile", "bird", "cat", "deer", "truck", "frog", "horse", "ship", "dog"]
environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
model = km.load_model("./saved_models/255100.h5")

def get_class(predictions):    
    for i in range(10):
        if int(predictions[i]) == 1:
            return catagories[i]

def predict(image: np.ndarray, x, y, w, h):
    roi = cv2.resize(image[y:(y+h), x:(x + w)], (32, 32)).reshape(-1, 32, 32, 3)
    pred = model.predict(roi)[0]
    return get_class(pred)


