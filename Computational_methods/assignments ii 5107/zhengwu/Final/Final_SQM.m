%final project
%Sequential Monte Carlo Method;

clear all;
close all;

load final_train.mat
%model identification
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

Ac = alpha(2:5,:);
Muc = alpha(1,:);

load final_test.mat

[M,c] = size(rate);
[M,d] = size(kin);

%sample size
N = 500;

%generate n samples
x_e(:,:,i) = rand(4,N);

for i=2:M   
    if (mod(i,100)==0)
        display(i);
    end;
    
    %generate the prediction
    tmp = mvnrnd(zeros(4,1),W,N)';
    
    x_tilte(:,:,i) = A*squeeze(x_e(:,:,i-1))+tmp;
    
   %compute the weights 
   %..........................................
   for t=1:N
   %For each sample compute lambda first and compute the log likelihood of possion
   Loglikel = zeros(d,1);
   for x =1:c
        lambda(i,x) = exp(Muc(1,x) + Ac(:,x)'*x_tilte(:,t,i));
        Loglikel(x,1) = log(lambda(i,x))*rate(i,x)-lambda(i,x)-log(factorial(rate(i,x)));
   end;
   w(i,t) = exp(sum(Loglikel));
   end;
   %..................................................
    
   %normalize the weight;
    w_n(i,:) = w(i,:)/sum(w(i,:));
    W_n(i,:) = cumsum(w_n(i,:));
    
    %estimate theta
    theta_hat(i,:) = w_n(i,:)*squeeze(x_tilte(:,:,i))';    

    %resample
    for t = 1:N
        U = rand;
        ind = find(U-W_n(i,:)<0);
        x_e(:,t,i) = x_tilte(:,ind(1),i);
    end;
end;


kin = kin - ones(M,1)*mean(kin);
for i=1:d
    subplot(4,1,i);
    plot(1:M,kin(:,i),1:M,theta_hat(:,i),'-.r');
    title('True and estimated state');
    legend('Estimated State','True state');
    legend('boxoff');
end;

%Rsq
Rsq = 1 - sum((theta_hat' - kin').^2,2)./sum(kin'.^2,2)


