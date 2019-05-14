from keras import models as km

import cv2
from os import environ

# TODO: order the classes correctly

catagories = [
'apple', # id 0
'aquarium_fish',
'baby',
'bear',
'beaver',
'bed',
'bee',
'beetle',
'bicycle',
'bottle',
'bowl',
'boy',
'bridge',
'bus',
'butterfly',
'camel',
'can',
'castle',
'caterpillar',
'cattle',
'chair',
'chimpanzee',
'clock',
'cloud',
'cockroach',
'couch',
'crab',
'crocodile',
'cup',
'dinosaur',
'dolphin',
'elephant',
'flatfish',
'forest',
'fox',
'girl',
'hamster',
'house',
'kangaroo',
'computer_keyboard',
'lamp',
'lawn_mower',
'leopard',
'lion',
'lizard',
'lobster',
'man',
'maple_tree',
'motorcycle',
'mountain',
'mouse',
'mushroom',
'oak_tree',
'orange',
'orchid',
'otter',
'palm_tree',
'pear',
'pickup_truck',
'pine_tree',
'plain',
'plate',
'poppy',
'porcupine',
'possum',
'rabbit',
'raccoon',
'ray',
'road',
'rocket',
'rose',
'sea',
'seal',
'shark',
'shrew',
'skunk',
'skyscraper',
'snail',
'snake',
'spider',
'squirrel',
'streetcar',
'sunflower',
'sweet_pepper',
'table',
'tank',
'telephone',
'television',
'tiger',
'tractor',
'train',
'trout',
'tulip',
'turtle',
'wardrobe',
'whale',
'willow_tree',
'wolf',
'woman',
'worm',
]

OUT_DIR = "../../images/out.jpg"
environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
model = km.load_model("../../NN/saved_models/100_22511.h5")

def get_max_index(pred):
    return max(enumerate(pred), key=lambda x: x[1])[0]    
    
def _predict():
    try:
        roi = cv2.imread(OUT_DIR).reshape(-1, 32, 32, 3)
        pred = list(model.predict(roi)[0])
        index = get_max_index(pred)
        return catagories[index], float(pred[index])
    except Exception as e:
        print(str(e))
        return "label", 0.0
