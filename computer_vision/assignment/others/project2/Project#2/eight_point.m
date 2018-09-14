%% Get user mouse click location which gives us pairs of correspondences

%load the data of 'rgb10.jpg' and 'rgb15.jpg' before the following
num_of_correspondence=9;
v1_images=ones(3,num_of_correspondence);
v2_images=ones(3,num_of_correspondence);
figure
imshow(rgb10);
figure
imshow(rgb15);
for i=1:num_of_correspondence
    figure(1);
    [v1_images(1,i), v1_images(2,i)] = ginput(1);
    figure(2);
    [v2_images(1,i), v2_images(2,i)] = ginput(1);
end
%% First we need to normalize the inputs
mean1=mean(v1_images');
mean2=mean(v2_images');
T1_trans=[1 0 -mean1(1);0 1 -mean1(2);0 0 1];
T2_trans=[1 0 -mean2(1);0 1 -mean2(2);0 0 1];

v1_centered=T1_trans*v1_images;
v1_rms=sqrt(sum(sum(v1_centered(1:2,:).^2))/size(v1_images,2));
T1_scale=[sqrt(2)/v1_rms 0 0; 0 sqrt(2)/v1_rms 0; 0 0 1];
v1_norm=T1_scale*v1_centered;
T1_norm=T1_scale*T1_trans;

v2_centered=T2_trans*v2_images;
v2_rms=sqrt(sum(sum(v2_centered(1:2,:).^2))/size(v2_images,2));
T2_scale=[sqrt(2)/v2_rms 0 0; 0 sqrt(2)/v2_rms 0; 0 0 1];
v2_norm=T2_scale*v2_centered;
T2_norm=T2_scale*T2_trans;

%% We need to construct the A matrix 
A=zeros([size(v1_norm,2) 9]);
for i=1:size(v1_norm,2)
    A(i,:)=[v1_norm(:,i)'*v2_norm(1,i) v1_norm(:,i)'*v2_norm(2,i) v1_norm(:,i)'];
end
[UA,SA,VA]=svd(A);
f_norm=VA(:,end);
F_norm=reshape(f_norm,[3 3])';
[UF,SF,VF]=svd(F_norm);
SF(3,3)=0.0; % We need to enforce the rank of F to be 2
F_norm=UF*SF*VF';
% We need to denormalize to get fundamental matrix F
F=T2_norm'*F_norm*T1_norm;

%% The essential matrix 
K=[-525 0 320; 0 -525 240; 0 0 1];
E=K'*F*K;
[U,S,V]=svd(E);
if det(U)<0
    U(:,3)=-U(:,3);
end
if det(V)<0
    V(:,3)=-V(:,3);
end
W=[0 -1 0; 1 0 0; 0 0 1];
R1=U*W*V';
R11=U*W'*V';
T1=U(:,3);
T11=-U(:,3);

R=R11;
T=T1;
XYZs_1=zeros(3,num_of_correspondence);
XYZs_2=zeros(3,num_of_correspondence);
for i=1:num_of_correspondence
    x=K\v1_images(:,i);
    x1=x(1)/x(3);
    y1=x(2)/x(3);
    x=K\v2_images(:,i);
    x2=x(1)/x(3);
    y2=x(2)/x(3);
    A1=[-1 0 x1 0;0 -1 y1 0;-R(1,1)+x2*R(3,1) -R(1,2)+x2*R(3,2) -R(1,3)+x2*R(3,3) -T(1)+x2*T(3);-R(2,1)+y2*R(3,1) -R(2,2)+y2*R(3,2) -R(2,3)+y2*R(3,3) -T(2)+y2*T(3)];
    [UA1,SA1,VA1]=svd(A1);
    XYZs_1(:,i)=VA1(1:3,4)/VA1(4,4);
    XYZs_2(:,i)=R*XYZs_1(:,i)+T;
end

%% 3d model of the desk
Desk=ones(3,8); % It has 8 vertices
figure
imshow(rgb10);
figure
imshow(rgb15);
% Pick the 4 visible vertex in order from "bottom front left", "top frot
% left", "top front right" to "top back right"
for j=1:4
    figure(1)
    xys_1= ginput(1);
    figure(2)
    xys_2= ginput(1);
    [Desk(:,j),View_s2]=get_3d_location(xys_1,xys_2);
end
% By the knowledge of parallelogram, we can find the coordinates of the fouth vertex
Desk(:,5) = Desk(:,1)-Desk(:,2)+Desk(:,3); % the "bottom front right" vertex
Desk(:,6) = Desk(:,5)-Desk(:,3)+Desk(:,4); % the "bottom back left" vertex
Desk(:,7) = Desk(:,2)-Desk(:,3)+Desk(:,4); % the "top back left" vertex
Desk(:,8) = Desk(:,7)-Desk(:,4)+Desk(:,6); % the "bottom back left" vertex
% Connect the triangles
Model_Desk=cell(6,1);
Model_Desk{1}=[Desk(:,1), Desk(:,2), Desk(:,3)];
Model_Desk{2}=[Desk(:,1), Desk(:,3), Desk(:,5)];
Model_Desk{3}=[Desk(:,2), Desk(:,3), Desk(:,4)];
Model_Desk{4}=[Desk(:,2), Desk(:,4), Desk(:,7)];
Model_Desk{5}=[Desk(:,3), Desk(:,4), Desk(:,5)];
Model_Desk{6}=[Desk(:,4), Desk(:,5), Desk(:,6)];
figure
for i=1:6
    triangle=Model_Desk{i};
    fill3(triangle(1,:),triangle(2,:),triangle(3,:),'r');
    hold on
end
%% 3d model of the book
Desk=ones(3,8); % It has 8 vertices
figure
imshow(rgb10);
figure
imshow(rgb15);
% Pick the 7 visible vertex in order
for j=1:7
    figure(1)
    xys_1= ginput(1);
    figure(2)
    xys_2= ginput(1);
    [Book(:,j),View_s2]=get_3d_location(xys_1,xys_2);
end
% By the knowledge of parallelogram, we can find the coordinates of the fouth vertex
Book(:,8) = Book(:,5)-Book(:,6)+Book(:,7); % the unvisible "bottom back left" vertex

% Connect the triangles
Model_Book=cell(6,1);
Model_Book{1}=[Book(:,1), Book(:,2), Book(:,3)];
Model_Book{2}=[Book(:,1), Book(:,3), Book(:,4)];
Model_Book{3}=[Book(:,1), Book(:,2), Book(:,5)];
Model_Book{4}=[Book(:,2), Book(:,5), Book(:,6)];
Model_Book{5}=[Book(:,2), Book(:,6), Book(:,3)];
Model_Book{6}=[Book(:,6), Book(:,3), Book(:,7)];
figure
for i=1:6
    triangle=Model_Book{i};
    fill3(triangle(1,:),triangle(2,:),triangle(3,:),'r');
    hold on
end