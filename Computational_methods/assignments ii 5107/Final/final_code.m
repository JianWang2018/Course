clear all; close all;

%%%%%%%%%% Model Identification %%%%%%%%%%%
load('C:\Users\xue\Desktop\final_train');

[M,C] = size(rate); % y
[M,d] = size(kin); % x

% centralize the kinematic data
kin = kin - ones(M,1) * mean(kin);

% MLE close formula to estimate the model parameters A, W
a = kin(2:M,:)';
b = kin(1:M-1,:)';

A = a * b' * inv(b*b');
W = (a - A * b) * (a - A * b)'/(M-1);

% Inhomogenous Poisson Process
kin = [ones(M,1), kin];

for i = 1:C
    % initial
    theta_old = zeros(d+1,1);
    theta_new = theta_old + 1;
    
    % Newton-Raphson Method
    while (norm(theta_new - theta_old)>1e-2)
        theta_old = theta_new;
        first_deriv = kin'* rate(:,i) - kin' * exp(kin*theta_old);
        second_deriv = - kin'.*(ones(d+1,1) * exp(kin*theta_old)')*kin;
        theta_new = theta_old - inv(second_deriv) * first_deriv;
    end
    
    theta(:,i) = theta_new;
    
end
    

%%%%%%%%%%%% Neural Decoding with Point Process Filter %%%%%%%%%%%%%%%%
clear('M','C','d','kin','rate');
load('C:\Users\xue\Desktop\final_test');

tic;

[M,C] = size(rate); % y
[M,d] = size(kin); % x

mu = theta(1,:);
alpha =theta(2:5,:);

W_kk_1(1,:,:) = eye(d);
W_kk(1,:,:) = eye(d);
X_kk_1(:,:) = zeros(d,1);
X_kk(:,:) = zeros(d,1);

for i = 2:M
    
    %% time update
    W_kk_1(i,:,:) = A * squeeze(W_kk(i-1,:,:)) * A' + W;
    X_kk_1(:,i) = A * X_kk(:,i-1);
    
    
    %% measurement update
         add1 = zeros(d,d);
     for c = 1:C
         add1 = add1 + alpha(:,c)* exp(mu(c)+alpha(:,c)'*X_kk_1(:,i))*alpha(:,c)';
     end
         W_kk(i,:,:) = inv(inv(squeeze(W_kk_1(i,:,:))) + add1);
         
     
         add2 = zeros(d,1);
     for c = 1:C
         add2 = add2 + (rate(i,c) - exp(mu(c) + alpha(:,c)' * X_kk_1(:,i)))*alpha(:,c);
     end
         X_kk(:,i) = X_kk_1(:,i) + squeeze(W_kk(i,:,:))* add2;
         
   
end
 
t1 = toc;

% R_square error
kin_mean = mean(kin);
kin = kin - ones(M,1) * kin_mean;

X_kk = X_kk + mean(kin)'* ones(1,M);
R_square = 1- sum((X_kk'-kin).^2)./sum(kin.^2);


% plot for the true and estimated position and velocity
figure(1);
tl = ['px','py','vx','vy'];
for i = 1:d
    subplot(4,1,i);
    plot(1:M,X_kk(i,:),1:M, kin(:,i)');
    xlabel('time');
    ylabel(tl(2*(i-1)+1:2*(i-1)+2));
    % legend('Point Process Filter estimation','True data');
end


%%%%%%%%%%%% Neural Decoding with Sequential Monte Carlo method %%%%%%%%%%%%%%%%
clear('M','C','d','kin','rate');
load('C:\Users\xue\Desktop\final_test');

tic;
[M,C] = size(rate); % y
[M,d] = size(kin); % x

n = 500;
X_h(:,:,1)=rand(d,n);
% X_h(:,:,i)=rand(d,n);

for i = 2:M
    
    % Prediction, generate the prediction set
    X_tilte(:,:,i) = A * squeeze(X_h(:,:,i-1)) + mvnrnd(zeros(d,1), W, n)';
    
    % Compute the weights
    for j = 1:n
        LL = zeros(d,1);
        for c = 1:C
            lamda(i,c) = exp(mu(1,c) + alpha(:,c)' * X_tilte(:,j,i));
            LL(c,1) = -lamda(i, c) + log(lamda(i, c))* rate(i,c) -log(factorial(rate(i,c)));
        end
        w(i,j) = exp(sum(LL));
    end
    
    w2(i,:) = w(i,:)/sum(w(i,:));
    weight(i,:) = cumsum(w2(i,:));
    theta_hat(i,:) = w2(i,:)* squeeze(X_tilte(:,:,i))';
    
    % resample
    for j=1:n
        U = rand;
        ind = find(U < weight(i,:));
        X_h(:,j,i) = X_tilte(:,ind(1),i);
    end
end

t2 = toc;

kin = kin - ones(M,1) * mean(kin);
R_square = 1- sum((theta_hat' - kin').^2,2)./sum(kin'.^2,2);

figure(2);
tl = ['px','py','vx','vy'];
for i = 1:d
    subplot(4,1,i);
    plot(1:M,theta_hat(:,i),1:M, kin(:,i)');
    xlabel('time(n=500)');
    ylabel(tl(2*(i-1)+1:2*(i-1)+2));
end

   

    




        
