FaceDetectMain.m is the main function to call finalFaceExtraction.m It reads raw image from inPath, crops the face and saves them in outPath. The corresponding bounding box information is saved in csvoutPath.
The finalFaceExtraction.m is a matlab function which detects facial landmarks and use them to crop faces. The function takes three input strings:
1. inPath -- the path to the folder of uncroppped images
2. csvoutPath -- the output path to the output csv file, which include file names and bounding box information
3. outPath -- the path to the folder of croppped faces

We included the mexw64 file in this folder, which are the functions we're using in the facial landmarks localizor. However, if any of them is not compatible with your computer, please run compile.m file, adjust the mode variable inside and recompile .cc file to .mexw64 format.

