% Local Feature Stencil Code
% CS 4495 / 6476: Computer Vision, Georgia Tech
% Written by James Hays

% 'features1' and 'features2' are the n x feature dimensionality features
%   from the two images.
% If you want to include geometric verification in this stage, you can add
% the x and y locations of the features as additional inputs.
%
% 'matches' is a k x 2 matrix, where k is the number of matches. The first
%   column is an index in features 1, the second column is an index
%   in features2. 
% 'Confidences' is a k x 1 matrix with a real valued confidence for every
%   match.
% 'matches' and 'confidences' can empty, e.g. 0x2 and 0x1.
function [matches, confidences] = match_features(features1, features2)

% This function does not need to be symmetric (e.g. it can produce
% different numbers of matches depending on the order of the arguments).

% To start with, simply implement the "ratio test", equation 4.18 in
% section 4.1.3 of Szeliski. For extra credit you can implement various
% forms of spatial verification of matches.

% Placeholder that you can delete. Random matches and confidences
num_features1 = size(features1, 1);
num_features2 = size(features2, 1);
matches = [];
confidences = [];

distances = zeros(num_features2, num_features1);
for i = 1:num_features1
    for j = 1:num_features2
        distances(j,i) = norm(features1(i,:)-features2(j,:));
    end
end

[sorted_dist, idx] = sort(distances);

for i = 1:num_features1
    ratio = sorted_dist(1,i) / sorted_dist(2,i);
    if ratio < 0.7
        matches(end+1,:) = [i,idx(1,i)];
        confidences(end+1) = 1-ratio;
    end
end
    
[confidences, idx] = sort(confidences, 'descend');
matches = matches(idx, :);

end

