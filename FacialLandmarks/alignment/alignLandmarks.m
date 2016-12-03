function landmark = alignLandmarks(inPathList, benchmark, outPath)

landmarks = cell(length(inPathList),3);
for i=1:length(inPathList)
    disp(['Processing ', inPathList{i}]);
    load(inPathList{i});
    T = compute_nonreflectiveSimilarity(final_align_info.landmark1_orig, benchmark, 0);
    mark = [final_align_info.landmark1_orig, ones(size(final_align_info.landmark1_orig,1),1)];
    landmark = mark * T;
    landmarks(i,:) = [inPathList(i), {final_align_info.landmark1_orig}, {landmark(:,1:2)}];
end

save(outPath, 'landmarks');
