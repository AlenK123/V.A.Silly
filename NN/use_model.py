from keras import models as km

import cv2
from os import environ

# TODO: order the classes correctly

OUT_DIR = "../../images/out.jpg"
catagories = ["airplane", "automobile", "bird", "cat", "deer", "dog", "frog", "horse", "ship", "truck"]
environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
model = km.load_model("../../NN/saved_models/255100.h5")

def get_max_index(pred):
    return max(enumerate(pred), key=lambda x: x[1])[0]    
    
def _predict():
    try:
        roi = cv2.imread(OUT_DIR).reshape(-1, 32, 32, 3)
        pred = list(model.predict(roi)[0])
        index = get_max_index(pred)
        return index, float(pred[index])
    except Exception as e:
        print(str(e))
        return "label", 0.0
