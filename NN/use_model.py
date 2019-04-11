from keras import models as km
import os
import sys
import cv2

# TODO: order the classes correctly

catagories = ["airplane", "automobile", "bird", "cat", "deer", "truck", "frog", "horse", "ship", "dog"]

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

if len(sys.argv) < 3:
    print("Not enough arguments")
    exit()
else:
    model_path = sys.argv[1]
    photo_path = sys.argv[2]

def prepare(photo_path):
    img_array = cv2.imread(photo_path, cv2.IMREAD_UNCHANGED)
    new_array = cv2.resize(img_array, (32, 32))
    return new_array.reshape(-1, 32, 32, 3)

def get_class(predictions):    
    for i in range(10):
        if int(predictions[i]) == 1:
            return catagories[i]

model = km.load_model(model_path)

print("predicting")

im_class = get_class(model.predict(prepare(photo_path))[0])

print(im_class)



