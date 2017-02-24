% Kalman Filter Algorithm
clear all; close all; clc

% A=[ 0.9  0.1 -0.2;
%    -0.3  0.8  0.1;
%     0.2  0.1  0.9]; 

A=[ 0.9  -0.2 -0.1;
   -0.2  0.9  0.3;
    0.0  0.3  0.6]; 
H=[ 1.0  0.5  0.2;
    0.5  1.0  0.1];
W=eye(3);
Q=50*eye(2);

% generate training data
x(:,1)=randn(3,1);
y(:,1)=H*x(:,1) + mvnrnd(zeros(1,2),Q)';
M=500;
for k=2:M
  x(:,k)=A*x(:,k-1)+mvnrnd(zeros(1,3),W)';
  y(:,k)=H*x(:,k) + mvnrnd(zeros(1,2),Q)';
end

figure(1)
plot(x')
xlabel('t','fontsize',16)
ylabel('x','fontsize',16)
legend('Component 1','Component 2','Component 3','location','northwest')
figure(2)
plot(y')
xlabel('t','fontsize',16)
ylabel('y','fontsize',16)
legend('Component 1','Component 2','location','northwest')


% estimation of A W H Q (assumed unknow)
Ah=x(:,2:M)*x(:,1:M-1)'*inv(x(:,1:M-1)*x(:,1:M-1)');
Wh=(x(:,2:M)*x(:,2:M)'-Ah*x(:,1:M-1)*x(:,2:M)')/(M-1);
Hh=y*x'*inv(x*x');
Qh=(y*y'-Hh*x*y')/M;

% generate test data
x2(:,1)=randn(3,1);
y2(:,1)=H*x2(:,1) + mvnrnd(zeros(1,2),Q)';
M2=500;
for k=2:M2
  x2(:,k)=A*x2(:,k-1)+mvnrnd(zeros(1,3),W)';
  y2(:,k)=H*x2(:,k) + mvnrnd(zeros(1,2),Q)';
end

% make inference using the Kalman Filter Algorithm
xh=zeros(3,1); xmh=xh; P=zeros(3); Pm=P; K=zeros(3,2);
for k=2:M
    xmh(:,k)=Ah*xh(:,k-1);
    Pm(:,:,k)=Ah*P(:,:,k-1)*Ah'+Wh;
    K(:,:,k)=Pm(:,:,k)*Hh'*inv(Hh*Pm(:,:,k)*Hh'+Qh);
    P(:,:,k)=(eye(3)-K(:,:,k)*Hh)*Pm(:,:,k);
    xh(:,k)=xmh(:,k)+K(:,:,k)*(y2(:,k)-Hh*xmh(:,k));
end

figure(2)
for i=1:3
    subplot(3,1,i)
    plot(1:M,x2(i,:), 1:M,xh(i,:));
    xlabel('t','fontsize',14)
    ylabel(['Component ' num2str(i)],'fontsize',12)
    legend('True Data','Kalman Filter Estimation','location','northwest')
end
%mtit('Plot of $\hat{x}_t$','Interpreter','latex','fontsize', 18)

% compute R_squares
R2m=1-sum((x2-xh).^2,2)./sum((x2-mean(x2,2)*ones(1,M)).^2,2);
R2m_s=1-sum(sum((x2-xh).^2))/sum(sum((x2-mean(x2,2)*ones(1,M)).^2));
R2p=1-sum((x2-xmh).^2,2)./sum((x2-mean(x2,2)*ones(1,M)).^2,2);
R2p_s=1-sum(sum((x2-xmh).^2))/sum(sum((x2-mean(x2,2)*ones(1,M)).^2));


