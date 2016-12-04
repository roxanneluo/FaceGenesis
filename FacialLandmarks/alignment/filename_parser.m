outfileList = cell(size(fileList));

for i=1:length(fileList)
    Name = fileList{i};
    slash = find(Name=='\');
    Name = Name(slash(end)+1 : end - 4);
    outfileList(i) = {Name};
end