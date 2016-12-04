% % sample expressions
% K = 6;
% [mu_id, eigen_id, mu_exp, eigen_exp, exp_labels, score_id, ...
%     score_exp, latent_id, latent_exp, neutral] = ComputeAll('KDEF_aligned.mat', K);
% 
% num_eigen_id = 40; num_eigen_exp = 40;
% eigen_id = eigen_id(:, 1:num_eigen_id); eigen_exp = eigen_exp(:,1:num_eigen_exp);
% score_id = score_id(:, 1:num_eigen_id); score_exp = score_exp(:, 1:num_eigen_exp);
% [exp_centers, covs] = cluster(score_exp, exp_labels, K+1);

identity = neutral(10,:);
coeffs_id = PCA.Project(identity, mu_id, eigen_id);
% draw all the centers
points = drawIDExpFromCoeffs(mu_id, mu_exp, eigen_id, eigen_exp, ...
    coeffs_id, exp_centers);

exp_idx = 7;
sample_coeffs = mvnrnd(exp_centers(exp_idx,:), covs(:,:,exp_idx), 10);
sample_points = drawIDExpFromCoeffs(mu_id, mu_exp, eigen_id, eigen_exp, ...
    coeffs_id, sample_coeffs);

load('KDEF_aligned.mat');
landmarks_exp = score_exp(label == exp_idx, :);
options = statset('Display','final');
max_k = 2;
AIC = zeros(1,max_k);
GMModels = cell(1,max_k);
for k = 1:max_k
    GMModels{k} = fitgmdist(landmarks_exp,k,'Options',options);
    AIC(k)= GMModels{k}.AIC;
end

[minAIC,numComponents] = min(AIC);
numComponents

BestModel = GMModels{numComponents}