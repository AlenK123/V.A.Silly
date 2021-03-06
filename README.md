
# V.A.Silly
<h3>Live Video Object Detection and Classification Program.</h3>

<h1>Abstract </h1>
This project is basically an R-CNN object detection and classification program using a live video feed from the  camera or a video using an object detection engine and a the <a href = 'https://www.cs.toronto.edu/~kriz/cifar.html'>Cifar100/Cifar10</a> neural networks for classification of said objects. <br>

The Neural networks were trained with the **Python** <a href='https://keras.io'> Keras library</a> to attain a Convolutional neural network. <br>

The largest part of the project is the object detection engine **(C++)** that uses a selective search algorithm to detect objects in the frame of a feed. using <a href='https://opencv.org/'> OpenCV</a>, we make region proposals and then, by using cool math and cool algorithms we reduce the regions to about less than 10-20% of the initial region proposals to more relevant regions. <a href='http://huppelen.nl/publications/selectiveSearchDraft.pdf'>Sources</a><br>

After gathering all of the regions we take every ROI and send it to the classifier for classification and confidence score of the network. We use only classification where the confidence score is larger than 0.4. so about 40% confidence score is fine in the meantime.<br>

After all of these boring stuff we draw a bounding box around the object and write the classification in the bounding box

## Files
<h3>NN/ </h3>
Files for the neural network part of the project.
<h3>image-processing/ </h3>
Source and Header files for the image processing part of the project and a makefile in the <b>src/</b> directory.
<h3>install/ </h3>
Script for preparing the executable files.
<h3>app/ </h3>
The JS app location.
<h3>.config/ </h3>
Home of the config file.
<h3>.log/ </h3>
Home of the error log file.

## Preparing and Compiling 
First you need to link some files to some directories.
`cd install && . ./install.sh`
Then you need to compile the binary using the Makefile
`cd ../image-processing/src && make`
Then you install the app dependencies
`cd ../../app && npm install`

## Dependencies
>**NodeJs**: Matirealized-css <br>
>**Python3**: Tensorflow2, Keras, OpenCV, Numpy, H5PY <br>
>**C++**: OpenCV4, Python C/C++ API <br>

### Notes
Probably works only for UNIX PC's
