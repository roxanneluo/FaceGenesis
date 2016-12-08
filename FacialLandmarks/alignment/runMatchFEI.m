root = 'E:\FaceAnalysis\FEI\NE_LM\';
root1 = 'E:\FaceAnalysis\FEI\neutral_proc\';

rooth = 'E:\FaceAnalysis\FEI\HA_LM\';
root1h = 'E:\FaceAnalysis\FEI\happy_proc\';

load('benchmark.mat')

list = dir([root, '*.mat']);
list1 = dir([root1, '*.mat']);
fileList = cell(0);
fileList1 = cell(0);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
    fileList1 = [fileList1, {[root1, list1(i).name]}];
end


listh = dir([rooth, '*.mat']);
list1h = dir([root1h, '*.mat']);
fileListh = cell(0);
fileList1h = cell(0);
for i=1:length(listh)
    fileListh = [fileListh, {[rooth, listh(i).name]}];
    fileList1h = [fileList1h, {[root1h, listh(i).name]}];
end

alignLandmarksFEI(fileList, fileList1, fileListh, fileList1h, meanFeat, 'FEI_aligned.mat');