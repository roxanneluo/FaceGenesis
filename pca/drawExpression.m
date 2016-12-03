function drawExpression(mu_id, mu_exp, eigen_exp, k, t)
    points = mu_id+mu_exp+k*eigen_exp;
    n = length(points)
    x = -points(1:n/2);
    y = -points(n/2+1: n);
    figure();
    scatter(x, y);
    title(t);
end