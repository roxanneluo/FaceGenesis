function finalFaceExtraction(inPath, outPath)

% compile.m should work for Linux and Mac.
% To Windows users:
% If you are using a Windows machine, please use the basic convolution (fconv.cc).
% This can be done by commenting out line 13 and uncommenting line 15 in
% compile.m

mkdir(outPath)

%compile;
load face_p146_small.mat


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

BoundingInfo = cell(0,6);
angleList = [0,-20,20,-10,10,90,-90,180];
ims = dir([inPath,'*.png']);
for i = 1:length(ims)
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
            disp('Trying difference model with other viewpoint...')
            count = count + 1;
            if count == length(angleList)
                imCenter = size(ori_im);
                imCenter = imCenter(1:2);
                width = min(imCenter)-2;
                imCenter = imCenter/2;
                rowMin = imCenter(1)-width/2/256*100;
                rowMax = imCenter(1)+width/2/256*100;
                colMin = imCenter(2)-width/2/256*100;
                colMax = imCenter(2)+width/2/256*100;
                
                BoundingInfo(i,2) = {colMin};
                BoundingInfo(i,3) = {rowMin};
                BoundingInfo(i,4) = {colMax-colMin};
                BoundingInfo(i,5) = {rowMax-rowMin};
                BoundingInfo(i,6) = {angleList(count)};
                
                
                
                outImage = imresize(ori_im(rowMin:rowMax,colMin:colMax,:),[256,256]);
                
                Name = ims(i).name;
                imwrite(outImage,[outPath,Name])
                
                finished = 1;
            end
        else
            %         % show highest scoring one
            %         figure,showboxes(im, bs(1),posemap),title('Highest scoring detection');
            %         % show all
            %         figure,showboxes(im, bs,posemap),title('All detections above the threshold');
            
            dist = [];
            if size(bs,2)>1
                for j = 1:size(bs,2)
                    Coord = bs(j).xy;
                    colMin = min((Coord(:,1)+Coord(:,3))/2);
                    colMax = max((Coord(:,1)+Coord(:,3))/2);
                    rowMax = max((Coord(:,2)+Coord(:,4))/2);
                    rowMin = min([Coord(:,2);Coord(:,4)]);
                    patchCenter = [(rowMin+rowMax)/2,(colMax+colMin)/2];
                    imCenter = size(ori_im);
                    imCenter = floor(imCenter(1:2)/2);
                    dist(j) = sqrt(sum((patchCenter-imCenter).^2));
                end
                [~,idx] = min(dist);
                Coord = bs(idx).xy;
            elseif size(bs,2)==1
                Coord = bs.xy;
            end
            
            
            
            for j = 1:size(bs,2)
                colMin = min((Coord(:,1)+Coord(:,3))/2);
                colMax = max((Coord(:,1)+Coord(:,3))/2);
                rowMax = max((Coord(:,2)+Coord(:,4))/2);
                rowMin = min([Coord(:,2);Coord(:,4)]);
                
                
                outImage = imresize(im(rowMin:rowMax,colMin:colMax,:),[256,256]);
                
                Name = ims(i).name;
                imwrite(outImage,[outPath,Name])
            end
            
            
            BoundingInfo(i,2) = {colMin};
            BoundingInfo(i,3) = {rowMin};
            BoundingInfo(i,4) = {colMax-colMin};
            BoundingInfo(i,5) = {rowMax-rowMin};
            BoundingInfo(i,6) = {angleList(count)};
            
            
            fprintf('Detection took %.1f seconds\n',dettime);
            
            finished = 1;
        end
        
    end
end
%cell2csv(csvName,BoundingInfo(:,1:end-1),',');
%cell2csv([csvName(1:end-4),'_Rotation.csv'],BoundingInfo,',');
