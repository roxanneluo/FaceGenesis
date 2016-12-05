
rootPathList = cell(0);
rootPathList(end+1) = {'E:\FaceAnalysis\KDEF_Proc\KDEF_frontProcXNE\'};

rootPathList1 = cell(0);
rootPathList1(end+1) = {'E:\FaceAnalysis\KDEF_Proc\Contour\XNE\'};

Feats = zeros(132,0);

fileName = cell(0);
for folder = 1:length(rootPathList)
    root = rootPathList{folder};
    root1 = rootPathList1{folder};
    list = dir([root, '*.mat']);
    for i=1:length(list)
        load([root, list(i).name]);
        load([root1, list(i).name]);
        
        feat = final_align_info.landmark1_orig;
        X = (bs.xy(52:end,1) + bs.xy(52:end,3) ) /2;
        Y = (bs.xy(52:end,2) + bs.xy(52:end,4) ) /2;
        feat = [[feat(:,1); X];[feat(:,2); Y]];
        Feats(:,end+1) = feat;
        
        fileName(end+1) = {list(i).name};
        disp(['File ', list(i).name, ' processed...'])
    end
    
end

meanFeat = mean(Feats')';
meanFeat = [meanFeat(1:66, :), meanFeat(67:end, :)];

save('benchmark.mat', 'meanFeat', 'fileName')

figure;
for i=1:66
    hold on;
    plot(meanFeat(i,1),meanFeat(i,2), '*')
    
end