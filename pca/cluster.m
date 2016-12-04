function [centers, covariances] = cluster(exp_coeffs, group_labels, K)
    d = size(exp_coeffs, 2);
    centers = zeros(K, d);
    covariances = zeros(d, d, K);
    for k = 1: K
       data = exp_coeffs(group_labels == k, :);
       centers(k, :) = mean(data, 1);
       covariances(:, :, k) = cov(data);
    end
end