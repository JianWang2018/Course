% Fundamental Matrix Stencil Code
% CS 4495 / 6476: Computer Vision, Georgia Tech
% Written by Henry Hu

% Returns the camera center matrix for a given projection matrix

% 'Points_a' is nx2 matrix of 2D coordinate of points on Image A
% 'Points_b' is nx2 matrix of 2D coordinate of points on Image B
% 'F_matrix' is 3x3 fundamental matrix

% Try to implement this function as efficiently as possible. It will be
% called repeatly for part III of the project
%{
function [ F_matrix ] = estimate_fundamental_matrix(Points_a,Points_b)

Points_num = size(Points_a,1);
A = []; B = -ones(Points_num,1);

for n = 1:Points_num
    u1 = Points_a(n,1);
    v1 = Points_a(n,2);
    u2 = Points_b(n,1);
    v2 = Points_b(n,2);
    A(end+1,:) = [u1*u2 v1*u2 u2 u1*v2 v1*v2 v2 u1 v1];
end

F_matrix = A\B;
F_matrix = [F_matrix;1];
F_matrix = reshape(F_matrix,[],3)';

[U,S,V] = svd(F_matrix);
S(3,3) = 0;
F_matrix = U*S*V';
        
end
%}

function [ F_matrix ] = estimate_fundamental_matrix(Points_a,Points_b)

Points_num = size(Points_a,1);
A = []; B = -ones(Points_num,1);

cu_a = sum(Points_a(:,1))/Points_num;
cv_a = sum(Points_a(:,2))/Points_num;
s = Points_num/sum(((Points_a(:,1)-cu_a).^2 + (Points_a(:,2)-cv_a).^2).^(1/2));
T_a = [s 0 0; 0 s 0; 0 0 1]*[1 0 -cu_a; 0 1 -cv_a; 0 0 1];
Points_a = T_a*[Points_a ones(Points_num,1)]';
Points_a = Points_a';

cu_b = sum(Points_b(:,1))/Points_num;
cv_b = sum(Points_b(:,2))/Points_num;
s = Points_num/sum(((Points_b(:,1)-cu_b).^2 + (Points_b(:,2)-cv_b).^2).^(1/2));
T_b = [s 0 0; 0 s 0; 0 0 1]*[1 0 -cu_b; 0 1 -cv_b; 0 0 1];
Points_b = T_b*[Points_b ones(Points_num,1)]';
Points_b = Points_b';

for n = 1:Points_num
    u1 = Points_a(n,1);
    v1 = Points_a(n,2);
    u2 = Points_b(n,1);
    v2 = Points_b(n,2);
    A(end+1,:) = [u1*u2 v1*u2 u2 u1*v2 v1*v2 v2 u1 v1];
end

F_matrix = A\B;
F_matrix = [F_matrix;1];
F_matrix = reshape(F_matrix,[],3);

F_matrix = T_a'*F_matrix*T_b;
F_matrix = F_matrix';

[U,S,V] = svd(F_matrix);
S(3,3) = 0;
F_matrix = U*S*V';
        
end

