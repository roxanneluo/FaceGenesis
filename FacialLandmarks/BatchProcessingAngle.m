% compile.m should work for Linux and Mac.
% To Windows users:
% If you are using a Windows machine, please use the basic convolution (fconv.cc).
% This can be done by commenting out line 13 and uncommenting line 15 in
% compile.m

inPath = 'E:\FaceAnalysis\ContestData\smiles_trset\';
outPath = 'E:\FaceAnalysis\ContestDataCropNew\smiles_trset\';
csvoutPath = 'E:\FaceAnalysis\ContestDataCropNew\smiles_trcsv\';
addpath('E:\LeeYuguang\MitosisExtraction\Toolbox\Matlab')

mkdir(outPath)
mkdir(csvoutPath)

%compile;

% load and visualize model
% Pre-trained model with 146 parts. Works best for faces larger than 80*80
load face_p146_small.mat

% % Pre-trained model with 99 parts. Works best for faces larger than 150*150
% load face_p99.mat

% % Pre-trained model with 1050 parts. Give best performance on localization, but very slow
% load multipie_independent.mat
% 
% disp('Model visualization');
% visualizemodel(model,1:13);
% disp('press any key to continue');
% pause;


% 5 levels for each octave
model.interval = 5;
% set up the threshold
model.thresh = min(-0.65, model.thresh);

% define the mapping from view-specific mixture id to viewpoint
if length(model.components)==13 
    posemap = 90:-15:-90;
elseif length(model.components)==18
    posemap = [90:-15:15 0 0 0 0 0 0 -15:-15:-90];
else
    error('Can not recognize this model');
end

BoundingInfo = cell(0,5);
angleList = [0,-20,20,-10,10];
ims = dir([inPath,'*.jpg']);
for i = 8:500
%     fprintf('testing: %d/%d\n', i, length(ims));
    BoundingInfo(i,1) = {ims(i).name};
    ori_im = imread([inPath,ims(i).name]);
    disp(['Processing Image ',ims(i).name])
    
    finished = 0; count = 1;
    
    while finished == 0
        im = imrotate(ori_im,angleList(count));
    tic;
    try
        bs = detect(im, model, model.thresh);
        bs = clipboxes(im, bs);
        bs = nms_face(bs,0.3);
    catch exception
        bs = zeros(0,0);
    end
    dettime = toc;
    if size(bs,1) == 0
        Coord = zeros(0);
    else
        Coord = bs.xy;
    end
    if size(bs,1) == 0 || size(Coord,1)<50
        disp('Face Detection Failed...')
        count = count + 1;
        if count == length(angleList)
            finished = 1;
        end
    else
%         % show highest scoring one
%         figure,showboxes(im, bs(1),posemap),title('Highest scoring detection');
%         % show all
%         figure,showboxes(im, bs,posemap),title('All detections above the threshold');
        
        

%         rowMin = min([Coord(:,1);Coord(:,3)]);
%         rowMax = max([Coord(:,1);Coord(:,3)]);
        
        for j = 1:size(bs,2)
        rowMin = min((Coord(:,1)+Coord(:,3))/2);
        rowMax = max((Coord(:,1)+Coord(:,3))/2);
%         colMin = min((Coord(:,2)+Coord(:,4))/2);
        colMax = max((Coord(:,2)+Coord(:,4))/2);
        colMin = min([Coord(:,2);Coord(:,4)]);
        
%         hold on
%         plot((Coord(:,1)+Coord(:,3))/2,(Coord(:,2)+Coord(:,4))/2,'r.','markersize',15);
        
        outImage = imresize(im(colMin:colMax,rowMin:rowMax,:),[256,256]);
        
        Name = ims(i).name;
        if size(bs,2)>1
            imwrite(outImage,[outPath,Name(1:end-4),'_',num2str(j),'.jpg'])
        else
            imwrite(outImage,[outPath,Name])
        end
        end

        BoundingInfo(i,2) = {colMin};
        BoundingInfo(i,3) = {colMax};
        BoundingInfo(i,4) = {rowMin};
        BoundingInfo(i,5) = {rowMax};
     
        
%         imshow(im(colMin:colMax,rowMin:rowMax,:))

       
        fprintf('Detection took %.1f seconds\n',dettime);
        
        finished = 1;
%         disp('press any key to continue');
%         pause;
%         close all;
    end
    
    end
end
disp('done!');
