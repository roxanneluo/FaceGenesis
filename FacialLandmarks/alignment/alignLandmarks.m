function landmark = alignLandmarks(inPathList, benchmark, outPath)

labels = ['XAF', 'XAN', 'XDI', 'XHA', 'XNE', 'XSA', 'XSU'];

landmark_orig = zeros(0,98);
landmark_aligned = zeros(0,98);
landmark_NE_aligned = zeros(0,98);
landmark_NE_orig = zeros(0,98);
fileName = cell(0);
fileName_NE = cell(0);
label = zeros(0);
label_NE = zeros(0);
for i=1:length(inPathList)
    disp(['Processing ', inPathList{i}]);
    load(inPathList{i});
    T = compute_nonreflectiveSimilarity(final_align_info.landmark1_orig, benchmark, 0);
    mark = [final_align_info.landmark1_orig, ones(size(final_align_info.landmark1_orig,1),1)];
    landmark = mark * T;
    
    Name = inPathList{i};
    slash = find(Name=='\');
    
    Name = Name(slash(end)+1 : end - 4);
    
    
    idx = findstr(labels, ['X',Name(end-2:end-1)]);
    idx = floor(idx / 3) + 1;
    aligned = [landmark(:,1);landmark(:,2)];
    orig = [final_align_info.landmark1_orig(:,1);final_align_info.landmark1_orig(:,2)];
    if idx == 5
        landmark_NE_aligned(end+1, :) = double(aligned)';
        landmark_NE_orig(end+1, :) = double(orig)';
        fileName_NE(end+1) = {Name};
        label_NE(end+1) = idx;
    else
        landmark_orig(end+1, :) = double(aligned)';
        landmark_aligned(end+1, :) = double(orig)';
        fileName(end+1) = {Name};
        label(end+1) = idx;
    end
end

fileName = fileName';
fileName_NE = fileName_NE';

save(outPath, 'landmark_orig', 'landmark_aligned', 'fileName', 'label', 'landmark_NE_orig', 'landmark_NE_aligned', 'fileName_NE', 'label_NE');
