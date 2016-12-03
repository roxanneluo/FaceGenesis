function drawIDs(mu_id, eigen_id, k_id, num_eigen_id_to_draw)
    title_pre = 'ID ';
    for i = num_eigen_id_to_draw:-1:1
        drawIDPixel(mu_id, eigen_id(:,i), k_id, [title_pre, 'eigen ', num2str(i)])
    end
end