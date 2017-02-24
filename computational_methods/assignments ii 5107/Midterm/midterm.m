clear all;close all;

S=load('C:\Documents and Settings\goober\Desktop\midterm_1_data.mat');

%sigma1 = 10;
%sigma1 = 20;
sigma1 = 100;
sigma2 = 30;

sigma = (sigma1*sigma2)/sqrt(sigma1^2+sigma2^2);

N=100;
X=S.D1;
[m,n]= size(X);
XX=X;

for i = 1:m
    for j = 1:n
        temp = [i-1,j; i+1,j; i,j-1; i,j+1];
        % ngh{i,j} = temp(min(temp,[],2)>= 1 & max(temp,[],2)<= n, :);
        ngh{i,j} = temp(min(temp,[],2)>= 1 & temp(:,1)<=m & temp(:,2)<=n, :);
    end
end

K=6;
for k=2:K
    for i=1:m
        for j=1:n
            mu1=0;
          for r=1:length(ngh{i,j})
                mu1 = mu1 + X(ngh{i,j}(r,1),ngh{i,j}(r,2));
          end
            mu1=mu1/r;
            mu2=X(i,j);
            
            mu=(mu1*sigma1^2+mu2*sigma2^2)/(sigma1^2+sigma2^2);
            U= mu + randn(1)*sigma;
            X(i,j)=U;      
                  
        end
    end
    XX(:,:,k) = X;
end

figure(1);
for k=1:K
    subplot(2,3,k);
    if(sum(sum(XX(:,:,k)))==N^2)
        image(255*ones(N,N));
    elseif(sum(sum(XX(:,:,k)))==-N^2)
        image(zeros(N,N));
    else
        imagesc(XX(:,:,k));
    end
    colormap(gray);
    title(sprintf('%d-th run',k));
 end



