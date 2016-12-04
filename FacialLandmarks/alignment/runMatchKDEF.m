root = 'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXAF\'

load('benchmark.mat')

list = dir([root, '*.mat']);
fileList = cell(0);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXAN\'
list = dir([root, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXDI\'
list = dir([root, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXHA\'
list = dir([root, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXNE\'
list = dir([root, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXSA\'
list = dir([root, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXSU\'
list = dir([root, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
end

alignLandmarks(fileList, meanFeat, 'KDEF_aligned.mat');