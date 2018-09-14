% Local Feature Stencil Code
% CS 4495 / 6476: Computer Vision, Georgia Tech
% Written by James Hays

% Returns a set of interest points for the input image

% 'image' can be grayscale or color, your choice.
% 'feature_width', in pixels, is the local feature width. It might be
%   useful in this function in order to (a) suppress boundary interest
%   points (where a feature wouldn't fit entirely in the image, anyway)
%   or(b) scale the image filters being used. Or you can ignore it.

% 'x' and 'y' are nx1 vectors of x and y coordinates of interest points.
% 'confidence' is an nx1 vector indicating the strength of the interest
%   point. You might use this later or not.
% 'scale' and 'orientation' are nx1 vectors indicating the scale and
%   orientation of each interest point. These are OPTIONAL. By default you
%   do not need to make scale and orientation invariant local features.
function [x, y, confidence, scale, orientation] = get_interest_points(image, feature_width)

% Implement the Harris corner detector (See Szeliski 4.1.1) to start with.
% You can create additional interest point detector functions (e.g. MSER)
% for extra credit.

% If you're finding spurious interest point detections near the boundaries,
% it is safe to simply suppress the gradients / corners near the edges of
% the image.

% The lecture slides and textbook are a bit vague on how to do the
% non-maximum suppression once you've thresholded the cornerness score.
% You are free to experiment. Here are some helpful functions:
%  BWLABEL and the newer BWCONNCOMP will find connected components in 
% thresholded binary image. You could, for instance, take the maximum value
% within each component.
%  COLFILT can be used to run a max() operator on each sliding window. You
% could use this to ensure that every interest point is at a local maximum
% of cornerness.


G=fspecial('gauss',[3, 3], 0.5);
[Gx,Gy] = gradient(G); 

gx = imfilter(image,Gx);
gy = imfilter(image,Gy);

gxx = imfilter(gx,Gx);
gxy = imfilter(gy,Gx);
gyy = imfilter(gy,Gy);

G = fspecial('gaussian',[16 16],2);
Ixx = imfilter(gxx,G);
Ixy = imfilter(gxy,G);
Iyy = imfilter(gyy,G);

Response = (Ixx.*Iyy)-Ixy.^2-0.06.*(Ixx+Iyy).^2;
Response(1 : 10, :) = 0;
Response(end - 10 : end, :) = 0;
Response(:, 1 : 10) = 0;
Response(:, end - 10 : end) = 0;

% choose the largest 200 response points as keypoints.
index = imregionalmax(Response);
sorted_resp = sort(Response(index),'descend');
pointnum = 1000;
top_resp = sorted_resp(1:pointnum);

x=[];y=[];
[height,width] = size(image);
for i = 1:pointnum
    [row,col]=find(Response==top_resp(i));
    x=[x; col];
    y=[y; row];
end

end

