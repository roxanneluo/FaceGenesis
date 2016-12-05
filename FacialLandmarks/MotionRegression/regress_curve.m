clear;
load('ck_grouped.mat');

features = cell(size(ck_data));
for i = 1:size(ck_data,1)
    data = ck_data{i,3};
    feature = zeros(size(data,1)-2, 12);
    
end