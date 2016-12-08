load('E:\LeeYuguang\ML_Project\FaceGenesis\FacialLandmarks\alignment\KDEF_aligned.mat');

addpath('E:\LeeYuguang\ML_Project\FaceGenesis\pca')

filename = 'E:\LeeYuguang\ML_Project\FaceGenesis\FacialLandmarks\alignment\KDEF_aligned.mat';
%[mu_id, eigen_id, mu_exp, eigen_exp, exp_labels, score_id, score_exp, latent_id, latent_exp, neutral] = ComputeAll(filename, 6);

%diff_eigen = (HA - NE) * eigen_exp;
%diff_eigen3 = diff_eigen(:,1:30);

models = cell(132,2);

NE = landmark_aligned(421:560, :);
HA = landmark_NE_aligned;


for i = 1:132
    disp(['Processing feature ', num2str(i)]);
    [B, FitInfo] = lasso(NE, HA(:,i));
    models(i,:) = [{B}, {FitInfo}];
end

save('linear_model132.mat', 'models');