#script di avvio

#!/bin/sh
module load agile-B24-r5

#export PFILES=/home/student/PARFILES

#Blob
cp ../AG_utils/include/Blob.h include/Blob.h
cp ../AG_utils/src/Blob.cpp src/Blob.cpp

# Blobs Finder
cp ../AG_utils/include/BlobsFinder.h include/BlobsFinder.h
cp ../AG_utils/src/BlobsFinder.cpp src/BlobsFinder.cpp


# Map Converter
cp ../AG_utils/include/FolderManager.h include/FolderManager.h
cp ../AG_utils/src/FolderManager.cpp src/FolderManager.cpp

# Folder Manager
cp ../AG_utils/include/MapConverter.h include/MapConverter.h
cp ../AG_utils/src/MapConverter.cpp src/MapConverter.cpp

make clean
make

./bin/AG_extspot-v2-bayesian-model-evaluator TrainingSetShortTerm 0.5 0.5 7.5

