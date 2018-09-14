% RANSAC Stencil Code
% CS 4495 / 6476: Computer Vision, Georgia Tech
% Written by Henry Hu

% Find the best fundamental matrix using RANSAC on potentially matching
% points

% 'matches_a' and 'matches_b' are the Nx2 coordinates of the possibly
% matching points from pic_a and pic_b. Each row is a correspondence (e.g.
% row 42 of matches_a is a point that corresponds to row 42 of matches_b.

% 'Best_Fmatrix' is the 3x3 fundamental matrix
% 'inliers_a' and 'inliers_b' are the Mx2 corresponding points (some subset
% of 'matches_a' and 'matches_b') that are inliers with respect to
% Best_Fmatrix.

% For this section, use RANSAC to find the best fundamental matrix by
% randomly sample interest points. You would reuse
% estimate_fundamental_matrix() from part 2 of this assignment.

% If you are trying to produce an uncluttered visualization of epipolar
% lines, you may want to return no more than 30 points for either left or
% right images.

function [ Best_Fmatrix, inliers_a, inliers_b] = ransac_fundamental_matrix(matches_a, matches_b)

matches_num = size(matches_a,1);
Best_count = 0; 

for iter = 1:5000
    sampled_idx = randsample(matches_num,8);
    Fmatrix = estimate_fundamental_matrix(matches_a(sampled_idx,:), matches_b(sampled_idx,:));
    in_a = []; in_b = [];
    count = 0;
    for n = 1:matches_num
        error = [matches_a(n,:) 1]*Fmatrix'*[matches_b(n,:) 1]';
        if abs(error) < 0.05
            in_a(end+1,:) = matches_a(n,:);
            in_b(end+1,:) = matches_b(n,:);
            count = count + 1;
        end
    end

    if count > Best_count
        Best_count = count;
        Best_Fmatrix = Fmatrix;
        inliers_a = in_a;
        inliers_b = in_b;
    end
end

idx = randsample(size(inliers_a,1),30);
inliers_a = inliers_a(idx,:);
inliers_b = inliers_b(idx,:);
end

