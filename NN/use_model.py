import numpy as np
import cv2
import json
from sys import argv
from sys import stderr
from keras import models as km

with open('../../.log/errlog') as errlog:
    stderr = errlog

conf = {}
with open(argv[1]) as f:
    conf = json.loads(f.read())

try:
    model = km.load_model(conf['model_path']) # loading the keras model
except Exception as e:
    print('[Python] ::', str(e), file=stderr) # for debbuging
    print('[Python] ::', str(e))

def get_max_index(pred):
    return max(enumerate(pred), key=lambda x: x[1])[0]    
    
def predict(binary):
    try:
        # taking the correct amount, 32 * 32 * 3 = 3072
        array = np.fromstring(binary, np.uint8)[ : 3072] 
        roi = array.reshape(-1, 32, 32, 3) # reshaping for the NN
        pred = list(model.predict(roi)[0]) # ignoring the np dtype
        index = get_max_index(pred) # taking the max prediction
        return index, float(pred[index])
    except Exception as e:
        print('[Python] ::', str(e), file=stderr) # for debbuging
        print('[Python] ::', str(e))
        # the program will ignore the low confidence prediction in a chance of error
        return "label", 0.0 
