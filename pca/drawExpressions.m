function drawExpressions(mu_id, mu_exp, eigen_exp, k_exp, num_exp_to_draw)
    title_pre = ['expression eigen '];
    for i = num_exp_to_draw:-1:1
        drawExpression(mu_id, mu_exp, eigen_exp(:,i), k_exp, [title_pre, num2str(i)]);
    end
end