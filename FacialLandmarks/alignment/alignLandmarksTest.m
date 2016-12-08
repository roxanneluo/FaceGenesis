function landmark = alignLandmarksTest(inPathList, inPathList1, benchmark, outPath)


landmark_orig = zeros(0,132);
landmark_aligned = zeros(0,132);
fileName = cell(0);
for i=1:length(inPathList)
    disp(['Processing ', inPathList{i}]);
    load(inPathList{i});
    load(inPathList1{i});
    if size(bs,2) > 1
        bs = bs(1);
    end
    X = (bs.xy(52:end,1) + bs.xy(52:end,3)) / 2;
    Y = (bs.xy(52:end,2) + bs.xy(52:end,4)) / 2;
    lmark = [final_align_info.landmark1_orig; [X, Y]];
    
    T = compute_nonreflectiveSimilarity(lmark, benchmark, 0);
    mark = [lmark, ones(size(lmark,1),1)];
    landmark = mark * T;
    
    Name = inPathList{i};
    slash = find(Name=='\');
    
    Name = Name(slash(end)+1 : end - 4);
    
    
    aligned = [landmark(:,1);landmark(:,2)];
    orig = [lmark(:,1);lmark(:,2)];
    landmark_orig(end+1, :) = double(aligned)';
    landmark_aligned(end+1, :) = double(orig)';
    fileName(end+1) = {Name};
end

fileName = fileName';

save(outPath, 'landmark_orig', 'landmark_aligned', 'fileName');
