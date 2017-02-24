clear all; close all;

% A = [0.9 0.1 -0.2; -0.3 0.8 0.1; 0.2 0.1 0.9];
A = [0.9 -0.2 -0.1; -0.2 0.9 0.3; 0.0 0.3 0.6];
H = [1 0.5 0.2; 0.5 1 0.1];
W = eye(3);
Q = 50*eye(2);

% set initial condition
x(:,1) = randn(3,1);
y(:,1) = H*x(:,1)+ mvnrnd(zeros(1,2),Q)';

% simulate training data
M=500;
for k=2:M
  x(:,k)=A*x(:,k-1)+mvnrnd(zeros(1,3),W)';
  y(:,k)=H*x(:,k) + mvnrnd(zeros(1,2),Q)';
end

% plots of Xt and Yt
figure(1);
subplot(2,1,1);
plot(x');
hleg1 = legend('First Component','Second Component','Third Component',...
              'Location','NorthEastOutside')
xlabel('time t');
ylabel('x');
subplot(2,1,2);
plot(y');
xlabel('time t');
ylabel('y');
hleg2 = legend('First Component','Second Component','Location','NorthEastOutside')

% estimates of A,W,H,Q (assume they are unknown)
Ahat = x(:,2:M)*x(:,1:M-1)'*inv(x(:,1:M-1)*x(:,1:M-1)')
What = (x(:,2:M)*x(:,2:M)'-Ahat*x(:,1:M-1)*x(:,2:M)')/(M-1);
Hhat = y*x'*inv(x*x');
Qhat = (y*y'-Hhat*x*y')/M;

% simulate another set of data(using true value of A, H, W, Q)
x2(:,1) = randn(3,1);
y2(:,1) = H*x2(:,1) + mvnrnd(zeros(1,2),Q)';
for k=2:M
  x2(:,k) = A*x2(:,k-1)+mvnrnd(zeros(1,3),W)';
  y2(:,k) = H*x2(:,k) + mvnrnd(zeros(1,2),Q)';
end

% estimate using Kalman Filter Algorithm (using estimated value of A, H, W, Q)
xh=zeros(3,1); 
xmh=xh;
P=zeros(3);
Pm=P; 
K=zeros(3,2);
for k=2:M
    xmh(:,k) = Ahat*xh(:,k-1);
    Pm(:,:,k) = Ahat*P(:,:,k-1)*Ahat'+What;
    K(:,:,k) = Pm(:,:,k)*Hhat'*inv(Hhat*Pm(:,:,k)*Hhat'+Qhat);
    P(:,:,k) = (eye(3)-K(:,:,k)*Hhat)*Pm(:,:,k);
    xh(:,k) = xmh(:,k)+K(:,:,k)*(y2(:,k)-Hhat*xmh(:,k));
end

figure(2)
for i=1:3
    subplot(3,1,i)
    plot(1:M,x2(i,:), 1:M,xh(i,:));
    xlabel('t')
    ylabel(['Component ' num2str(i)])
    legend('True Data','Kalman Filter Estimation','location','northwest')
end

% compute R_squares_error
R2m=1-sum((x2-xh).^2,2)./sum((x2-mean(x2,2)*ones(1,M)).^2,2); % filtering estimation
R2p=1-sum((x2-xmh).^2,2)./sum((x2-mean(x2,2)*ones(1,M)).^2,2); % prediction estimation


 
