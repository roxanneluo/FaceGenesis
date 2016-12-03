function drawID(mu_id, eigen, k, t)
    points_xy = mu_id + k * eigen;
    n = length(points_xy);
    point_x = -points_xy(1:n/2);
    point_y = -points_xy(n/2+1:n);
    figure();
    scatter(point_x, point_y);
    title(t)
end