
rootPathList = cell(0);
rootPathList(end+1) = {'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXNE\'};
Feats = zeros(98,0);

for folder = 1:length(rootPathList)
    root = rootPathList{folder};
    list = dir([root, '*.mat']);
    for i=1:length(list)
        load([root, list(i).name]);
        
        feat = final_align_info.landmark1_orig;
        feat = [feat(:,1);feat(:,2)];
        Feats(:,end+1) = feat;
        
        disp(['File ', list(i).name, ' processed...'])
    end
    
end

meanFeat = mean(Feats')';
meanFeat = [meanFeat(1:49, :), meanFeat(50:end, :)];

save('benchmark.mat', 'meanFeat')

figure;
for i=1:49
    hold on;
    plot(meanFeat(i,1),meanFeat(i,2), '*')
    
end