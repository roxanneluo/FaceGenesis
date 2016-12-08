root = 'E:\LeeYuguang\ML_Project\FaceGenesis\data\testPicLM\';
root1 = 'E:\LeeYuguang\ML_Project\FaceGenesis\data\testPicContour\';

load('benchmark.mat')

list = dir([root, '*.mat']);
list1 = dir([root1, '*.mat']);
fileList = cell(0);
fileList1 = cell(0);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
    fileList1 = [fileList1, {[root1, list1(i).name]}];
end

alignLandmarksTest(fileList, fileList1, meanFeat, 'Test_aligned.mat');