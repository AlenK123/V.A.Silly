#!/bin/bash
mkdir ../.config
printf '{"model_path" : "" }\n' >> ../.config/config.json
ln -s ../../NN/use_model.py ../image-processing/src/use_model.py 
