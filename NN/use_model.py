from keras import models as km
import cv2
from os import environ

# TODO: order the classes correctly

OUT_DIR = "../../images/out.jpg"
catagories = ["airplane", "automobile", "bird", "cat", "deer", "dog", "frog", "horse", "ship", "truck"]
environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
model = km.load_model("../../NN/saved_models/255100.h5")

def get_class(predictions):    
    for i in range(10):
        if int(predictions[i]) == 1:
            return catagories[i]

def _predict(a):
    try:
        roi = cv2.imread(OUT_DIR).reshape(-1, 32, 32, 3)
        pred = model.predict(roi)[0]
        return get_class(pred)
    except:
        return "label"