matPath = 'E:\FaceAnalysis\ContestDataCropNew\BoundingBox\BInfo.mat';
matPath1 = 'E:\FaceAnalysis\ContestDataCropNew\BoundingBox\BInfo1.mat';

load(matPath)

for i = 1:length(BoundingInfoLocal)
    try
    Name = BoundingInfoLocal{i,1};
    Number = str2num(Name(11:15));
    BoundingInfo(Number,:) = BoundingInfoLocal(i,1:5);
    catch exception
    end
end

save(matPath1,'BoundingInfo');