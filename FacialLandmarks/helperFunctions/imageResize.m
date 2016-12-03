inPath = 'E:\FaceAnalysis\ContestDataProc\Gender\LFM_res\';
outPath = 'E:\FaceAnalysis\ContestDataProc\Gender\LFM_resA\';


folder = dir(inPath);
folder = folder(3:end);
folder = {folder.name};

for i = 1:length(folder)
    savePath = [outPath,folder{i},'\'];
    loadPath = [inPath,folder{i},'\'];
    mkdir(savePath);
    loadFiles = dir(loadPath);
    loadFiles = loadFiles(3:end);
    loadFiles = {loadFiles.name};
    for j = 1:length(loadFiles)
    loadFilePath = [loadPath,loadFiles{j}];
    saveFilePath = [savePath,loadFiles{j}];
    
    Image = imread(loadFilePath);
    Image = imresize(Image,[256,256]);
    
    imwrite(Image,saveFilePath);
    end
end