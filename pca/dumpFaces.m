function dumpFaces(mu_id, mu_exp, eigen_id, eigen_exp, landmarks_id, coeffs_exp, img_id, folder_path)
    n_id = size(landmarks_id,1);
    n_exp = size(coeffs_exp, 1);
    assert(size(img_id, 1) == n_id);
    row = 100; col = 100;
    mkdir(folder_path);
    for id = 1:n_id
        id_folder_name = [folder_path,'\', num2str(id),'\'];
        mkdir(id_folder_name);
        imwrite(reshape(img_id(id, :), row, col), [id_folder_name, num2str(id), '.png']);
        dumpFace([id_folder_name, '\landmarks.txt'], landmarks_id(id,:)');
        for exp=1:n_exp
            points = landmarks_id(id,:)'+mu_exp+eigen_exp*coeffs_exp(exp,:)';
            dumpFace([id_folder_name, num2str(exp), '.txt'], points);
        end
    end
end

% x = xy(1,:);
% y = xy(2,:);
function dumpFace(filename, points)
    n = length(points);
    x = points(1:n/2);
    y = points(n/2+1:n);
    fileID = fopen(filename, 'w');
    fprintf(fileID, '%f, %f\n', [x';y']);
    fclose(fileID);
end