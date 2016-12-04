function render(img, landmarks, new_landmarks)
    row = 100; col = 100;
    original_x = landmarks(1:49)'/2-15; original_y = landmarks(50:end)'/2-20;
    xx = new_landmarks(1:49)/2-15; yy = new_landmarks(50:end)/2-15;
    Fx = scatteredInterpolant(xx, yy, original_x, 'natural', 'none');
    Fy = scatteredInterpolant(xx, yy, original_y, 'natural', 'none');
    [img_x, img_y] = meshgrid(linspace(min(xx), max(xx), 100), linspace(min(yy), max(yy), 100));
    c_x = Fx(img_x, img_y);
    c_y = Fy(img_x, img_y);
    c_x(c_x < 0 | c_x>= col) = NaN;
    c_y(c_y < 0 | c_y >= row) = NaN;
    c = int32(floor(c_x)*col+floor((c_y)))+1;
    cmap = repmat(double(img)'/255.0, 1, 3);
    figure();
%     surf(img_x, img_y, ones(size(img_x)));
    % h = findobj('Type', 'surface');
    % set(h, 'CData', c, 'FaceColor', 'texturemap');
    % colormap(map);
    surf(-img_x, -img_y, zeros(size(img_x)), reshape(cmap(c(:),:), size(c, 1), size(c, 2), 3), ...
        'linestyle', 'none')
    view([0 90])
    hold on
    grid off
    scatter(-xx, -yy)

% tri = delaunay(xx,yy);
% trisurf(tri,xx,yy, ones(49,1),1:49);
% colors = zeros(49, 1);
% F  = zeros(49, 1);
% row = 100; col = 100;
% for i=1:length(xx)
%     F(i) = floor(yy(i)/4+0.5)*row+floor((xx(i)/4+0.5));
%    %colors(i) =  img(F(i,1), F(i,2));
% end
% h = findobj('Type','surface');
% set(h,'CData',1:numel(img),'FaceColor','texturemap')
% colormap(repmat(img(:)/255,1,3))

% figure;
% patch('Faces',tri,'Vertices',[xx; yy]','FaceVertexCData',F,'FaceColor','texturemap');
% colormap (repmat(img, 1, 3));
% % load earth % Load image data, X, and colormap, map
% sphere; h = findobj('Type','surface');
% % hemisphere = [ones(257,125),X,ones(257,125)];
% % set(h,'CData',flipud(hemisphere),'FaceColor','texturemap')
% % colormap(map)
% % axis equal
% % view([90 0])
end