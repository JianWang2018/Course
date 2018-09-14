% This function use the result (R,T) in eight point algorithm and the given
% camera intrinsic matrix K, transfor the coordinates of two corresponding
% pixel points to two 3d coordinates of two cameras 

function [XYZs_1,XYZs_2]=get_3d_location(xys_1,xys_2)
R=[0.816469717915658,-0.206449333203566,-0.539217834039759;0.100765789914859,0.970507826284323,-0.218999576948710;0.568527444640591,0.124471811831766,0.813193281298952];
T=[0.901860912020436;0.364894452578877;0.231298365421871];

K=[-525,0,320;0,-525,240;0,0,1];
xyzs_1=[xys_1(1),xys_1(2),1]';
xyzs_2=[xys_2(1),xys_2(2),1]';
x=K\xyzs_1;
x1=x(1)/x(3);
y1=x(2)/x(3);
x=K\xyzs_2;
x2=x(1)/x(3);
y2=x(2)/x(3);
A=[-1 0 x1 0;0 -1 y1 0;-R(1,1)+x2*R(3,1) -R(1,2)+x2*R(3,2) -R(1,3)+x2*R(3,3) -T(1)+x2*T(3);-R(2,1)+y2*R(3,1) -R(2,2)+y2*R(3,2) -R(2,3)+y2*R(3,3) -T(2)+y2*T(3)];
[UA,SA,VA]=svd(A);
XYZs_1=VA(1:3,4)/VA(4,4);
XYZs_2=R*XYZs_1+T;
end