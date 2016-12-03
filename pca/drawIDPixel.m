function drawIDPixel(mu_id, eigen, k, t)
    pixels = mu_id + k * eigen;
    n = sqrt(length(pixels));
    figure();
    imshow(reshape(pixels, n, n))
    title(t)
end