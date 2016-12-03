function T = compute_nonreflectiveSimilarity(uv,xy,ignoreRotation)

pts1=uv';
pts2=xy';
mean1 = mean(pts1,2);
mean2 = mean(pts2,2);

var1 = sum(((pts1(1,:)-mean1(1)).^2+(pts1(2,:)-mean1(2)).^2))/length(pts1);
% var2 = sum(((pts2(1,:)-mean2(1)).^2+(pts2(2,:)-mean2(2)).^2))/length(pts1);
cov12 = zeros(2,2);
for i = 1:length(pts1)
    cov12 = cov12 + (pts2(:,i)-mean2)*(pts1(:,i)-mean1)';
end
cov12 = cov12./length(pts1);
[U,D,V] = svd(cov12);
if det(U)*det(V) < 0.0001 || det(U)*det(V) > -0.0001 %rounding errors for det(U)*det(V)
    S = eye(2);
else
    S = [1,0;0,-1];
end
if ignoreRotation
    R=[1 0 ; 0 1];
else
    R = U*S*V';
end
c = trace(D*S)/var1;
t = mean2 - c*R*mean1;
T = [c*R, t]';
T(:,3)=[0 0 1]';
%trans = maketform('affine', T);
