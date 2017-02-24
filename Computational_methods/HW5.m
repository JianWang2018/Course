clear all; close all;
N=10;
X = sign(randn(N,N));
XX = X;

%H = -1;J = 0;
%H = 0;J = 0;
%H = 1;J = 0;
%H = 0;J = -1;
%H = 0;J = 1;
%H = 4;J = -1;
H = 4;J = -2;
 
% define the neighbors
for i = 1:N
    for j = 1:N
        temp = [i-1,j;i+1,j;i,j-1;i,j+1];
        ngh{i,j}=temp(min(temp,[],2)>= 1 & max(temp,[],2)<= N, :);
    end
end
rand('seed',0);
K=9;
for k=2:K
    for i=1:N
        for j=1:N
        sn=H;
        for r=1:length(ngh{i,j})
            sn = sn+ J*X(ngh{i,j}(r,1), ngh{i,j}(r,2));
        end
        P1=exp(sn*2)/(1+exp(2*sn));
        U=rand;
        X(i,j)=(U<P1)-(U>P1);
    end
end
XX(:,:,k)=X;
end
 
for k=1:K
    subplot(3,3,k);
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
