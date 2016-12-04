function points = drawIDExpFromCoeffs(mu_id, mu_exp, eigen_id, eigen_exp, coeffs_id, coeffs_exp)
    n = length(mu_id);
    figure();
    n_id = size(coeffs_id,1);
    n_exp = size(coeffs_exp, 1);
    points = zeros(n_id, n_exp, n);
    for id = 1:n_id
        for exp=1:n_exp
            points(id, exp,:) = mu_id+mu_exp+coeffs_id(id,:)*eigen_id'...
                +coeffs_exp(exp,:)*eigen_exp';
            x = -points(id, exp, 1:n/2);
            y = -points(id, exp, n/2+1:n);
            subplot(n_id, n_exp, (id-1)*n_exp+exp);
            scatter(x,y, 1);
            axis off
            axis equal
        end
    end
end