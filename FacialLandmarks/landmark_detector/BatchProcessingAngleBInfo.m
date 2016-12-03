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

BoundingInfoLocal = cell(0,6);
angleList = [0,-20,20,-10,10];
ims = dir([inPath,'*.jpg']);
count = 1;
for i = 1:500
%     fprintf('testing: %d/%d\n', i, length(ims));
    BoundingInfoLocal(i,1) = {ims(i).name};
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
        rowMin = min((Coord(:,1)+Coord(:,3))/2);
        rowMax = max((Coord(:,1)+Coord(:,3))/2);
%         colMin = min((Coord(:,2)+Coord(:,4))/2);
        colMax = max((Coord(:,2)+Coord(:,4))/2);
        colMin = min([Coord(:,2);Coord(:,4)]);
        
%         hold on
%         plot((Coord(:,1)+Coord(:,3))/2,(Coord(:,2)+Coord(:,4))/2,'r.','markersize',15);
        
        outImage = imresize(im(colMin:colMax,rowMin:rowMax,:),[256,256]);
        
%         imwrite(outImage,[outPath,ims(i).name])

        BoundingInfoLocal(i,2) = {colMin};
        BoundingInfoLocal(i,3) = {colMax};
        BoundingInfoLocal(i,4) = {rowMin};
        BoundingInfoLocal(i,5) = {rowMax};
        BoundingInfoLocal(i,6) = {i};
     
        
%         imshow(im(colMin:colMax,rowMin:rowMax,:))

       
        fprintf('Detection took %.1f seconds\n',dettime);
        
        finished = 1;
%         disp('press any key to continue');
%         pause;
%         close all;
    end
    
    if floor(double(i)/50)*50 == i
        try
            load([csvoutPath,'BInfo.mat']);
            for j = 1:size(BoundingInfoLocal,1)
                BoundingInfo(BoundingInfoLocal{j,6},:) = BoundingInfoLocal(j,1:5);
            end
            save([csvoutPath,'BInfo.mat'],'BoundingInfo');
%             cell2csv([csvoutPath,'BInfo.csv'],BoundingInfo,',');
        catch exception
            pause(5)
            disp('Pausing for 15 secs....')
            try
            save([csvoutPath,'BInfo.mat'],'BoundingInfo');
%             cell2csv([csvoutPath,'BInfo.csv'],BoundingInfo,',');
            catch exception
                disp('Saving attempt failed.....')
            end
        end
    end
    
    
    end
end
disp('done!');
