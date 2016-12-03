inPath = 'E:\deepalia\FERG_GRAIL\siggraph\result\hj_644\';
outPath = 'E:\deepalia\FERG_GRAIL\siggraph\result\hj_644\Proc\';
%csvoutPath = 'E:\LeeYuguang\Face\sideCode\HeadHunter_PO-CR\HeadHunter_PO-CR\sample_img\Test.csv';

mkdir(inPath)
mkdir(outPath)
finalFaceExtraction(inPath, outPath);