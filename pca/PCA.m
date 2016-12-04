classdef PCA
   methods(Static) 
       % point and mu are all row vectors
       function coeffs = Project(points, mu, eigens)
           size(points)
           size(mu)
           n = size(points, 1);
           coeffs =(points - repmat(mu, n, 1)) * eigens;
       end
   end
end