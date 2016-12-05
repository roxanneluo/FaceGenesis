load('ck_grouped.mat')

idx = [1, 5, 21, 25, 34, 41];
idx = [idx, idx + 49]

figure;

for p = [1:100]
data = ck_data{p,3};

data = data(1:end,idx);

for i = 1:size(data,2)/2
    if abs(data(end,i) - data(1,i)) > 2 & abs(data(end,i+size(data,2)/2) - data(1,i+size(data,2)/2)) > 2
    X = (data(:,i)' - data(1,i)) / (data(end,i) - data(1,i));
    Y = (data(:,i+size(data,2)/2)' - data(1,i+size(data,2)/2)) / (data(end,i+size(data,2)/2) - data(1,i+size(data,2)/2));
    
    X1 = X(2:end);
    X2 = X(1:end-1);
    maxX = max(X2-X1);
    
    Y1 = [Y, 0];
    Y2 = [0, Y];
    maxY = max(Y2-Y1);
    
    if maxX <= 1.2 & maxY <= 1.2
    
    proc = [[0:1/(size(data,1)-1):1]; log(1./X-1); log(1./Y-1)];
    %proc = [[0:1/(size(data,1)-1):1]; X; Y];
    %proc = [[0:1/(size(data,1)-1):1]; asin(X); asin(Y)];
    hold on;
    plot3(proc(1,:), proc(2,:), proc(3,:), '.');
    end
    end
end


end

%ylim([0,1]);