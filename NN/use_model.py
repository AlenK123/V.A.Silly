from keras import models as km
import cv2
import json

conf = {}
with open('../../.config/config.json') as f:
    conf = json.loads(f.read())

out_dir = '/tmp/out.jpg'
model = km.load_model(conf['model_path'])

def get_max_index(pred):
    return max(enumerate(pred), key=lambda x: x[1])[0]    
    
def _predict():
    try:
        roi = cv2.imread(out_dir).reshape(-1, 32, 32, 3)
        pred = list(model.predict(roi)[0])
        index = get_max_index(pred)
        return index, float(pred[index])
    except Exception as e:
        print(str(e))
        return "label", 0.0
