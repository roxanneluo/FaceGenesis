function drawIDExp(mu_id, mu_exp, eigen_id, eigen_exp,k_id,k_exp, id_range, exp_range)
    n = length(mu_id);
    figure();
    n_id = length(id_range);
    n_exp = length(exp_range);
    for id = id_range
        for exp=exp_range
            points = mu_id+mu_exp+k_id*eigen_id(:,id)+k_exp*eigen_exp(:,exp);
            x = -points(1:n/2);
            y = -points(n/2+1:n);
            subplot(n_id, n_exp, (id-1)*n_exp+exp);
            scatter(x,y, 1);
            axis off
        end
    end
end