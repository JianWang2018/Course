clear all;
close all;
%Final Project
load('final_train.mat');

[M,c] = size(rate);
[M,d] = size(kin);

%center data
kin = kin-ones(M,1)*mean(kin);

%with MLE to find A and W;
X_a = kin(2:M,:)';
X_b = kin(1:M-1,:)';

A = X_a*X_b'*inv(X_b*X_b');
W = (X_a-A*X_b)*(X_a-A*X_b)'/(M-1);

%Inhomogeneous Possion process
kin=[ones(M,1),kin];

for i=1:c
    %initial
    alpha_o = zeros(d+1,1);
    alpha_n = alpha_o + 1; 
    
    
    %use Newton-Raphson method to solve MLE
    while(norm(alpha_n-alpha_o)>1e-2)
        alpha_o = alpha_n;
        
        fdd=kin'*rate(:,i)-kin'*exp(kin*alpha_o); %first
        
        sdd = -kin'.*(ones(d+1, 1)* exp(kin * alpha_o)')*kin; %second;
        
        alpha_n = alpha_o - inv(sdd)*fdd;
        
    end;
    
    alpha(:,i) = alpha_n;
    
end;

%point process Filter algorithm

load ('final_test.mat');
[M,c] = size(rate);
[M,d] = size(kin);

kin = kin-ones(M,1)*mean(kin);
kin = kin';
%initial W_kt(4 by 4) and W_km(4 by 4)
W_kt(1,:,:) = eye(4);
W_km(1,:,:) = eye(4);
X_kt(:,:) = zeros(d,1);
X_km(:,:) = zeros(d,1);

Ac = alpha(2:5,:);
Muc = alpha(1,:);

for i=2:M

    %time update:
    W_kt(i,:,:) = A*squeeze(W_km(i-1,:,:))*A' + W;
    X_kt(:,i) = A*X_km(:,i-1);
    
    %measurement update;
    
    %compute second part of measurement update;
    Lp = zeros(4,4);
    
    for t=1:c
        Lp = Lp + Ac(:,t)*exp(Muc(t)+Ac(:,t)'*X_kt(:,i))*Ac(:,t)';
    end;
    
    W_km(i,:,:)=inv(inv(squeeze(W_kt(i,:,:)))+ Lp);
    
    XLp = zeros(d,1);
    
    for t=1:c
        XLp = XLp + (rate(i,t) - exp (Muc(t)+Ac(:,t)'*X_kt(:,i)))*Ac(:,t);
    end;
    
    X_km(:,i) = X_kt(:,i) + squeeze(W_km(i,:,:))*XLp;
    
end;

%Rsqure
Rsq = 1 - sum((X_km - kin).^2,2)./sum(kin.^2,2);  

% plot figure;
for i=1:d
subplot(4,1,i)
plot(1:M,X_km(i,1:M),'-.r');
hold on;
plot(1:M,kin(i,1:M));
title('True and estimated state');
legend('Estimated State','True state','boxoff')
end;

print -depsc PointF