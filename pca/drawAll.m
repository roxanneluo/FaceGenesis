% mu + eigen_id * alpha_id + mu_exp + eigen_exp * alpha_exp
function [mu_id, eigen_id, mu_exp, eigen_exp] = computeAll(filename)
    [neutral, mu_id, expressions, mu_exp] = preprocess(filename);
    [eigen_id, eigen_exp] = MMD(neutral, expressions);
end
function [eigen_id, eigen_exp] = MMD(neutral, expressions)
    [eigen_id, score_id, latent_id] = pca(neutral);
    figure();
    plot(latent_id);
    [eigen_exp, score_exp, latent_exp] = pca(expressions);
    figure();
    plot(latent_exp);
end
function [neutral, mu_id, expressions, mu_exp] = preprocess(filename)
    data = load(filename)
end
