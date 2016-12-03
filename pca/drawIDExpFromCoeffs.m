function drawIDExpFromCoeffs(mu_id, mu_exp, eigen_id, eigen_exp, coeffs_id, coeffs_exp)
    n = length(mu_id);
    figure();
    n_id = size(coeffs_id,1);
    n_exp = size(coeffs_exp, 1);
    for id = 1:n_id
        for exp=1:n_exp
            points = mu_id+mu_exp+eigen_id*coeffs_id(id,:)'+eigen_exp*coeffs_exp(exp,:)';
            x = -points(1:n/2);
            y = -points(n/2+1:n);
            subplot(n_id, n_exp, (id-1)*n_exp+exp);
            scatter(x,y, 1);
            axis off
            axis equal
        end
    end
end