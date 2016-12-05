clear all
load('../alignment/CK_aligned.mat');


ck_data = cell(0,3);
cur = 0;
curData = zeros(0,98);
for i = 1:length(landmarks)
    fileName = landmarks{i,1};
    slash = find(fileName=='\');
    fileName = fileName(slash(end)+1 : end - 4);
    personID = str2num(fileName(2:4));
    groupID = str2num(fileName(6:8));
    ID = personID * 1000 + groupID;
    if i==1
        cur = ID;
    end
    if ID ~= cur | i == length(landmarks)
        ck_data = [ck_data; [{floor(cur / 1000)}, {mod(cur,1000)}, {curData}] ];
        curData = zeros(0,98);
        cur = ID;
        disp(['Processed person ', fileName(1:8)]);
    end
    curData = [curData; [landmarks{i,3}(:,1);landmarks{i,3}(:,2)]'];
    
end

save('ck_grouped.mat', 'ck_data')