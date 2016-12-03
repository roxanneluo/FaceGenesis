csvPath = 'E:\FaceAnalysis\ContestData\gender_fex_valset.csv';
rootPath = 'E:\FaceAnalysis\ContestDataCropNew\FINAL\GenderNew\All\';
addpath('E:\LeeYuguang\MitosisExtraction\Toolbox\Matlab')
outPath = 'E:\FaceAnalysis\ContestDataCropNew\FINAL\SmileNew\val\';

Info = csv2cell(csvPath,'fromfile');

malePath = [outPath,'non_smile\'];
femalePath = [outPath,'smile\'];

mkdir(malePath)
mkdir(femalePath)

fileNames = Info(2:end,1);
gender = Info(2:end,7);


for i = 1:length(fileNames)
    if str2num(gender{i}) == 0
        try
            copyfile([rootPath,fileNames{i}],[malePath,fileNames{i}])
        catch exception
        end
    else str2num(gender{i}) == 1
        try
            copyfile([rootPath,fileNames{i}],[femalePath,fileNames{i}])
        catch exception
        end
    end
end