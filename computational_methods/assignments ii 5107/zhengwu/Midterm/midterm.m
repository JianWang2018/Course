%for the midterm

I=load('midterm_1_data.mat');


%take D1 as an exmple

D1=I.D1;

[m n] = size(D1);

% Do gaussian Smoothing....
for blur=2:2:12
h = fspecial('gaussian',10,0.1*blur);
I1 = imfilter(D1,h);

%Display the images
Handle=figure(1);
subplot(2,3,blur/2)
imagesc(I1);
colormap(gray);
title(sprintf('%d-th run', blur));
end;
print(Handle,'-depsc','Figure1.eps');






%define the neighbor
for i = 1:m
    for j = 1:n
        temp = [i-1,j; i+1,j; i,j-1; i,j+1];
        ngh{i,j} = temp(min(temp, [], 2) >= 1 & temp(:, 1) <= m & temp(:,2)<=n, :);
    end
end

%define the sd for the posterior density

del1 = 10; %20 100;
del1 = 20;
del1 = 100;

del2 = 30; 

del = del1*del2/sqrt((del1^2+del2^2));

%K sweeps
K = 6;
XX = D1;
X = D1;

for k=2:K
for i = 1:m
    for j = 1:n
        mu1 = 0;
        for r=1:length(ngh{i,j})
                mu1 = mu1 + X(ngh{i,j}(r,1),ngh{i,j}(r,2));
        end
        
        mu1 = mu1/r;
        
        mu2 = X(i,j);  %some problem here
        
        %new mu for the posterior Density
        mu = (mu1*del2^2 + mu2*del1^2)/(del1^2 + del2^2);
        
        %Use Gibbs sampling
        U = mu + randn(1)*del;
        
        X(i,j)=U;
    end;
end;
  XX(:,:,k) =X;
end;




figure(1);
for k=1:K
    hold on, subplot(2,3,k);
    imagesc(XX(:,:,k));
    colormap(gray);
    title(sprintf('%d-th run', k));
   % pause; clf;
end
    

        