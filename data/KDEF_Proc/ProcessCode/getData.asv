root = 'E:\FaceAnalysis\KDEF_Proc\';

list = dir(root);
list = list(3:end);

n = size(list,1);

r = rand([1, 140]);
r = r > 0.75;

featsTr = zeros(0, 49*2+1);
    featsTe = zeros(0, 49*2+1);
    
    pixelFeatsTr = zeros(0, 10001);
    pixelFeatsTe = zeros(0, 10001);

for i = [1:n-1]
    Name = list(i).name;
    Path = [root, Name, '\'];
    
    locList = dir([Path,'*.mat']);
    imgList = dir([Path,'*.jpg']);
    N = size(locList,1);
    
    for j=[1:N]
        disp(['Processing file ', locList(j).name])
        fileName = [Path, locList(j).name];
        load(fileName);
        fileName = [Path, imgList(j).name];
        Image = imread(fileName);
        Image = rgb2gray(Image);
        Image = imresize(Image, [100,100]);
        Image = reshape(Image, [1, 10000]);
        if (r(j)==1)
            featsTr = [featsTr; [i;final_outLandmark(:,1);final_outLandmark(:,2)]'];
            pixelFeatsTr = [pixelFeatsTr; [i, Image]];
        else
            featsTe = [featsTe; [i; final_outLandmark(:,1);final_outLandmark(:,2)]'];
            pixelFeatsTe = [pixelFeatsTe; [i ,Image]];
        end
        
    end
    
end

    save('Tr.mat', 'featsTr', 'pixelFeatsTr');
    save('Te.mat', 'featsTe', 'pixelFeatsTe');