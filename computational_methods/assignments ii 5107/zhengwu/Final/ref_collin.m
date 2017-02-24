clear all;
close all;

%Model Identification

load final_train.mat

[M,C] = size(rate);
d = size(kin,2);
kin_mean = mean(kin);
kin = kin - ones(M,1) * kin_mean;


a = kin(2:M, :)';
b = kin(1:M-1,:)';
A = a * b'* inv(b*b');
W = (a - A * b)*(a - A * b)'/(M-1);

kin = [ones(M,1) kin];
% Fit the measurement model using MLE
for ( c=1:C )
    alpha_old = zeros(d+1,1);
    alpha_new = alpha_old + 1;
    
   % Newton-Raphson Method
   count(c) = 0;
   while ( norm(alpha_new - alpha_old) > 1e-2 )
       count(c) = count(c) + 1;
       alpha_old = alpha_new;
       nmt = kin' * rate (:,c) -  kin'*exp(kin * alpha_old); %First LL derivative
       dnt = -kin'.*(ones(d+1, 1)* exp(kin * alpha_old)')*kin; %Second LL derivative
       alpha_new = alpha_old - dnt\nmt; %New alpha
   end
   
   alpha(:,c)= alpha_new; 
   
end

%%

% Neural Decoding
load final_test.mat

Mt = size(rate,1);
x_m(:,1) = zeros(d,1);
x(:,1) = x_m(:,1); %x{k-1|k-1}
P_m(:,:,1) = zeros(d); 
P(:,:,1) = zeros(d); %W{k-1 |k-1}

for ( k = 2:Mt)
    P_m(:,:,k) = A * P(:,:,k-1) * A'+ W; %W{k |k-1}
    x_m(:,k) = A * x(:,k-1); %x{k |k-1}
    P(:,:,k) = inv( inv(P_m(:,:,k))+ alpha(2:end,:)*diag(exp([1 x_m(:,k)']*alpha))*alpha(2:end,:)'); 
    
    TMP=zeros(d,1);
    for i=1:C
        TMP = TMP + ( rate(k,i)-exp( alpha(1,i)+ alpha(2:end,i)'*x_m(:,k)) )*alpha(2:end,i);
    end;
    x(:,k) = x_m(:,k)+squeeze(P(:,:,k))*TMP; %%% Problem here
end
        
x = x + kin_mean' *ones(1,Mt);
        
%R-squared error
Rsq = 1 - sum((x' - kin).^2)./sum(kin.^2);  
        
for (i = 1:d)
   subplot(2,2,i) %Plot for each component of x{k}
   plot(1:Mt, x(i,:), 1:Mt, kin(:,i)');
end


%%

% %Sequential Monte Carlo
% load final_test.mat
%         
% Mt = size(rate,1);
% k = 20; 
% s = zeros(d,k,Mt);
% weight = ones(k, Mt)/k;
% Weight = cumsum(weight);
% xh = zeros(d,Mt);
%         
%      for( t = 2:Mt)
%          
%          if( mod (t,100) == 0 )
%             dis(sprintf('t=%d',t)); %
%          end
%             
%          r = rand(1,k);
%          j = sum(weight(:,t-1) *ones(1,k)< ones(k,1)*r + 1; %
%          s_p = s(:,k,t-1);
%      end

