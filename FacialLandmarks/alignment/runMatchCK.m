root = 'E:\FaceAnalysis\KDEF_frontImages\XAF\'

load('benchmark.mat')

list = dir([root, '*.mat']);
fileList = cell(0);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_frontImages\XAF\'
list = dir([root, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_frontImages\XAF\'
list = dir([root, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_frontImages\XAF\'
list = dir([root, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_frontImages\XAF\'
list = dir([root, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_frontImages\XAF\'
list = dir([root, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_frontImages\XAF\'
list = dir([root, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
end

alignLandmarks(fileList, meanFeat, 'CK_aligned.mat');