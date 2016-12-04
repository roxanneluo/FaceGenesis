% mu + eigen_id * alpha_id + mu_exp + eigen_exp * alpha_exp
function [mu_id, eigen_id, mu_exp, eigen_exp, exp_labels, score_id, score_exp, latent_id, latent_exp, neutral] = ComputeAll(filename, k)
    [neutral, mu_id, expressions, mu_exp, exp_labels] = preprocess(filename, k);
    [eigen_id, eigen_exp, score_id, score_exp, latent_id, latent_exp] = MMD(neutral, expressions);
end
function [eigen_id, eigen_exp, score_id, score_exp, latent_id, latent_exp] = MMD(neutral, expressions)
    [eigen_id, score_id, latent_id] = pca(neutral);
    figure();
    plot(latent_id);
    title('Singular Values for Identity')
    [eigen_exp, score_exp, latent_exp] = pca(expressions);
    figure();
    plot(latent_exp);
    title('Singular Values for Expressions')
end
%{
function [neutral, mu_id, expressions, mu_exp, exp_labels] = preprocess(filename, k)
    data = load(filename);
    data = data.featsTe;
    d = size(data, 2) - 1
    
    neutral_class_id = 5;
    neutral_IX = (data(:, 1) == neutral_class_id);
    %neutral = im2double(data(neutral_IX, 2:end));
    neutral = data(neutral_IX, 2:end);
    
    mu_id = mean(neutral, 1);
    n_id = size(neutral, 1)
    neutral = neutral - ones(n_id, 1) * mu_id;
    mu_id = mu_id';
    
    n_exp = size(data, 1) - n_id
    expressions = zeros(n_exp, d);
    exp_labels = zeros(n_exp, 1);
    index_start  = 1;
    for i = 1:k
        if (i == neutral_class_id)
            continue;
        end
        expression = data(data(:,1) == i, 2:end);
        %expression = im2double(data(data(:,1) == i, 2:end));
        expression = expression - neutral;
        n = size(expression,1);
        expressions(index_start: index_start + n -1, :) = expression;
        exp_labels(index_start:index_start+n-1, :) = i;
        index_start = index_start + n;
    end
    mu_exp = mean(expressions, 1);
    expressions = expressions - ones(size(expressions, 1), 1) * mu_exp;
    mu_exp = mu_exp';
end
%}

function [neutral, mu_id, expressions, mu_exp, exp_labels] = preprocess(filename, k)
    dataset = load(filename);
    neutral = dataset.landmark_NE_aligned;
    expressions = dataset.landmark_aligned - repmat(neutral, k, 1);
    mu_id = mean(neutral, 1);
    mu_exp = mean(expressions, 1);
    exp_labels = dataset.label;
end