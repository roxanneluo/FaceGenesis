rootPath = 'E:\FaceAnalysis\ContestData\smiles_valset\';
outPath = 'E:\FaceAnalysis\ContestCrop\smiles_valset\';
csvFile = 'E:\FaceAnalysis\ContestData\gender_fex_valset.csv';
addpath('E:\LeeYuguang\MitosisExtraction\Toolbox\Matlab')

labels = csv2cell(csvFile,'fromfile');
% fileList = dir([rootPath,'*.jpg']);
% fileList = {fileList.name};


labels = labels(2:end,:);
fileName = labels(:,1);
Bbox = cellfun(@str2num,labels(:,2:5));
Gender = cellfun(@str2num,labels(:,6));
Xpre = cellfun(@str2num,labels(:,7));

mkdir(outPath)

for i = 1:length(labels)
    if floor((i-1)/1000)*1000 == (i-1)
    disp(['Processing image ', num2str(i,'%04d')])
    end
    Image = imread([rootPath,fileName{i}]);
    Cimage = Image(Bbox(i,2):(Bbox(i,2)+Bbox(i,4)),Bbox(i,1):(Bbox(i,1)+Bbox(i,3)),:);
    
    imwrite(Cimage,[outPath,fileName{i}]);
    
    %     
%     if ~isempty(find(strcmp(fileList,{Name})))
%         if labels(i,1)==1
%             movefile([rootPath,Name],[posPath,Name]);
%         else
%             movefile([rootPath,Name],[negPath,Name]);
%         end
%     end
end
