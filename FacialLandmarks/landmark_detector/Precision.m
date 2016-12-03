predPath = 'E:\LeeYuguang\Face\sideCode\face-release1.0-basic\face-release1.0-basic\finalSubmission\Predict.csv';
truePath = 'E:\FaceAnalysis\ContestData\gender_fex_valset.csv';

predVal = csv2cell(predPath,'fromfile');
trueVal = csv2cell(truePath,'fromfile');

Nametrue = trueVal(2:end,1);
Namepred = predVal(2:end,1);

predVal = predVal(2:end,(end-1):end);
trueVal = trueVal(2:end,(end-1):end);

for i = 1:length(Nametrue)
    Name = Nametrue{i,1};
    Numtrue(i) = str2num(Name(11:15));
end

for i = 1:length(Namepred)
    Name = Namepred{i,1};
    Numpred(i) = str2num(Name(11:15));
end

Numpred(end) = 6172;

correctNum = 0;
correctNum1 = 0;
correctNum2 = 0;


for i = 1:length(predVal)
    j = find(Numtrue==Numpred(i));
    try
    if (predVal{i,1} == trueVal{j,1} ||trueVal{j,1}==2)&& (predVal{i,2} == trueVal{j,2}||trueVal{j,2}==2)
        correctNum = correctNum + 1;
    end
    catch exception
        correctNum = correctNum + 1;
    end
    
    
    try
    if (predVal{i,1} == trueVal{j,1} ||trueVal{j,1}==2)
        correctNum1 = correctNum1 + 1;
    end
    catch exception
        correctNum1 = correctNum1 + 1;
    end
    
    
    try
    if (predVal{i,2} == trueVal{j,2}||trueVal{j,2}==2)
        correctNum2 = correctNum2 + 1;
    end
    catch exception
        correctNum2 = correctNum2 + 1;
    end
end