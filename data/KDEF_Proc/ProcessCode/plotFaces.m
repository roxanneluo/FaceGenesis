root = 'E:\FaceAnalysis\KDEF_Proc\';

list = dir(root);
list = list(3:end);

n = size(list,1);

outImage = uint8(zeros(300, 700, 3));

for i = 1:n-1
    Name = list(i).name;
    Path = [root, Name, '\'];
    imgList = dir([Path,'*.jpg']);
    
    for j=1:3
        fileName = [Path, imgList(j).name];
        Image = imread(fileName);
        Image = imresize(Image, [100,100]);
        outImage(j*100-99:j*100, i*100-99:i*100, :) = Image;
    end
end

imwrite(outImage, 'faceSamples.png');