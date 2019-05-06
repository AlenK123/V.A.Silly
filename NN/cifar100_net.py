'''
    my take on the cifar100 database neural net
'''

import os
from numpy.random import seed
from keras.callbacks import EarlyStopping
from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Flatten
from keras.layers.convolutional import Conv2D
from keras.optimizers import Adam
from keras.layers.pooling import MaxPooling2D
from keras.utils import to_categorical
from keras.datasets import cifar100

seed(1000)

save_dir = os.path.join(os.getcwd(), 'saved_models')

model_name = '100_22511.h5'

(X_train, Y_train), (X_test, Y_test) = cifar100.load_data()

model = Sequential()

model.add(Conv2D(128, (3, 3), activation='elu', padding='same', input_shape=X_train.shape[1:]))
model.add(Conv2D(128, (3, 3), activation='elu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.1))

model.add(Conv2D(256, (3, 3), padding='same', activation='elu'))
model.add(Conv2D(256, (3, 3), activation='elu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.25))

model.add(Conv2D(512, (3, 3), padding='same', activation='elu'))
model.add(Conv2D(512, (3, 3), activation='elu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.5))

model.add(Flatten())
model.add(Dense(1024, activation='elu'))
model.add(Dropout(0.5))
model.add(Dense(100, activation='softmax'))


# Compile the model
model.compile(loss='categorical_crossentropy',
                optimizer=Adam(lr=0.0001, decay=1e-6),
                metrics=['accuracy'])

# Train the model
model.fit(X_train / 255.0, to_categorical(Y_train),
            batch_size=128,
            shuffle=True,
            epochs=250,
            validation_data=(X_test / 255.0, to_categorical(Y_test)),
            callbacks=[EarlyStopping(min_delta=0.001, patience=3)])

# Evaluate the model
scores = model.evaluate(X_test / 255.0, to_categorical(Y_test))

print('Loss: %.3f' % scores[0])
print('Accuracy: %.3f' % scores[1])

# Save model and weights
if not os.path.isdir(save_dir):
    os.makedirs(save_dir)

model_path = os.path.join(save_dir, model_name)
model.save(model_path)
print('Saved trained model at %s ' % model_path)

with open(os.path.join(save_dir, "accuracy.txt"), "a") as f:
    f.write('for ' + model_name + ':\nTest loss: ' + str(scores[0]) + '\nTest accuracy: ' + str(scores[1]) + '\n\n')