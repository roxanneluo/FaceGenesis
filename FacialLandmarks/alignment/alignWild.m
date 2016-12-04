close all
oriRoot = 'E:\FaceAnalysis\CK_Images\001\';
matRoot = 'E:\FaceAnalysis\CK_Proc001\';
data1Name = 'S014_001_00000001';
data2Name = 'S014_001_00000028';

MatName1 = [dataRoot, data1Name, '.mat'];
MatName2 = [dataRoot, data2Name, '.mat'];
ImageName1 = [oriRoot, data1Name, '.png'];
ImageName2 = [oriRoot, data2Name, '.png'];

landmark_idx = [11, 12, 13, 14, 20, 23, 26, 29];

Image1 = imread(ImageName1);
load(MatName1);
alignInfo1 = final_align_info;
landmark1 = alignInfo1.landmark1_orig;
alignface1 = final_align_face;
imshow(Image1);
hold on;
for i=1:49
    plot(landmark1(i,1), landmark1(i,2),'*');
    hold on
end
pt1 = landmark1(landmark_idx,:);

Image2 = imread(ImageName2);
load(MatName2);
alignInfo2 = final_align_info;
landmark2 = alignInfo2.landmark1_orig;
alignface2 = final_align_face;
figure;
imshow(Image2);
hold on;
for i=1:49
    plot(landmark2(i,1), landmark2(i,2),'*');
    hold on
end
pt2 = landmark2(landmark_idx,:);

T = compute_nonreflectiveSimilarity(pt1,pt2,0);
landmark1A = [landmark1, ones(size(landmark1,1),1)];
temp = landmark1A * T;

translation = mean(pt2) - mean(pt1);
landmark2_trans = [landmark2(:,1) - translation(1), landmark2(:,2) - translation(2)];
scale = std(landmark2(:)) / std(pt1(:));
%translate = mean(landmark1 - landmark2_trans / scale);

%landmark1_scaled = landmark1 - translate;

figure;
imshow(Image2);
hold on;
for i=1:49
    plot(temp(i,1), temp(i,2),'*');
    hold on
end
pt2 = landmark2(landmark_idx,:);