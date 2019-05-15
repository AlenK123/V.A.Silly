#!/bin/bash
mkdir ../images
mkdir ../.log
touch ../.log/log.cverrlog
ln -s ../../NN/use_model.py ../image-processing/src/use_model.py 
ln -s ../../NN/use_100_model.py ../image-processing/src/use_100_model.py
