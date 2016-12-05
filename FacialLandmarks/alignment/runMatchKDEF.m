root = 'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXAF\';
root1 = 'E:\FaceAnalysis\KDEF_Proc\Contour\XAF\';

load('benchmark.mat')

list = dir([root, '*.mat']);
list1 = dir([root1, '*.mat']);
fileList = cell(0);
fileList1 = cell(0);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
    fileList1 = [fileList1, {[root1, list1(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXAN\'
root1 = 'E:\FaceAnalysis\KDEF_Proc\Contour\XAN\';
list = dir([root, '*.mat']);
list1 = dir([root1, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
    fileList1 = [fileList1, {[root1, list1(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXDI\'
root1 = 'E:\FaceAnalysis\KDEF_Proc\Contour\XDI\';
list = dir([root, '*.mat']);
list1 = dir([root1, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
    fileList1 = [fileList1, {[root1, list1(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXHA\'
root1 = 'E:\FaceAnalysis\KDEF_Proc\Contour\XHA\';
list = dir([root, '*.mat']);
list1 = dir([root1, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
    fileList1 = [fileList1, {[root1, list1(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXNE\'
root1 = 'E:\FaceAnalysis\KDEF_Proc\Contour\XNE\';
list = dir([root, '*.mat']);
list1 = dir([root1, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
    fileList1 = [fileList1, {[root1, list1(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXSA\'
root1 = 'E:\FaceAnalysis\KDEF_Proc\Contour\XSA\';
list = dir([root, '*.mat']);
list1 = dir([root1, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
    fileList1 = [fileList1, {[root1, list1(i).name]}];
end

root = 'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXSU\'
root1 = 'E:\FaceAnalysis\KDEF_Proc\Contour\XSU\';
list = dir([root, '*.mat']);
list1 = dir([root1, '*.mat']);
for i=1:length(list)
    fileList = [fileList, {[root, list(i).name]}];
    fileList1 = [fileList1, {[root1, list1(i).name]}];
end

alignLandmarksKDEF(fileList, fileList1, meanFeat, 'KDEF_aligned.mat');