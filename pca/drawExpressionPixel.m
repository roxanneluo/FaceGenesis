function drawExpressionPixel(mu_id, mu_exp, eigen_exp, k, t)
    pixels = mu_id+mu_exp+k*eigen_exp;
    n = sqrt(length(pixels));
    figure();
    imshow(reshape(pixels, n, n))
    title(t)
end