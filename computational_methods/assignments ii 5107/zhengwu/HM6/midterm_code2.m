
clear; close all; clc;
load midterm_2_train.mat

% % 0 plot raw data z, neuron firing rate
% figure(1)
% ribbon(rate)
% colormap hsv
% xlabel('index of neuron, 1:42')
% ylabel('time series kT, k=1:3100, T=70ms')
% zlabel('rate of spike:')

% 1 estimate parameters A, H, W, Q
x=kin';
y=rate';
M = 3100;
Ae = x(:,2:M)*x(:,1:M-1)' * inv(x(:,1:M-1)*x(:,1:M-1)');
We=1/(M-1)*(x(:,2:M)*x(:,2:M)'-Ae*x(:,1:M-1)*x(:,2:M)');
He=y*x'*inv(x*x');
Qe=1/M*(y*y'-He*x*y');




%tic;
% 2 using kalman filter to estimate xt
clear ('x','kin','y','rate','M');
load midterm_2_test.mat %midterm_2_train.mat %
x2=kin';
y2=rate';
M=910;
% x2=x;
% y2=y;
xh=[11;11;0.3;-0.5];
xmh=xh; P=1*eye(4); Pm=P; K=1*ones(4,42);
for k=2:910
    xmh(:,k)=Ae*xh(:,k-1);
    Pm(:,:,k)=Ae*P(:,:,k-1)*Ae'+We;
    K(:,:,k)=Pm(:,:,k)*He'*inv(He*Pm(:,:,k)*He'+Qe);
    P(:,:,k)=(eye(4)-K(:,:,k)*He)*Pm(:,:,k);
    xh(:,k)=xmh(:,k)+K(:,:,k)*(y2(:,k)-He*xmh(:,k));
end

% figure(2)
% hold on;
% M1=1;
% M2=3100;
% tl = ['px','py','vx','vy'];
% for i=1:4
%     subplot(4,1,i)
%     plot(1:M,x2(i,:), 1:M,xh(i,:));
%     xlabel('time')
%     %ylim([-1e2, 1e2])
%     xlim([0,M])
%     ylabel(tl(2*(i-1)+1:2*(i-1)+2),'fontsize',12)
%     legend('True Data','Kalman Filter Estimation')
% end

% compute R2 error of filtering estimation
M=910;
R2m  =1-sum((x2-xh).^2,2)./sum((x2-mean(x2,2)*ones(1,M)).^2,2);
%toc;




tic
% 3 estimate x using Sequential Monte Carlo Methods
clear ('x2','kin','y2','rate','xh','xmh','P','Pm','K','M1','M2','k');
load midterm_2_test.mat
x3=kin';
y3=rate';
We4=We;
% (1) initialization x as random numbers
n=20;
u=mean(x3,2);
% x(:,:,1) = mvnrnd(Ae*u, round(We*10000)/10000,n)

for i=1:n % n particles
    x(:,i,1) = u+rand(4,1); % 
end


for t=1:909
    % (2) prediction, generate the prediction set
    for i=1:n
        xh(:,i,t+1) = Ae*x(:,i,t) + mvnrnd(zeros(1,4),(We4+We4')/2)';
    end
    % (3) update: compute w(t+1,i) and normalize
    for i=1:n
        w(t+1,i) = mvnpdf(y3(:,t+1), He*xh(:,i,t+1),(Qe+Qe')/2);
    end
    wh(t+1,:) = w(t+1,:)/sum(w(t+1,:));
    Wh(t+1,:) = cumsum(wh(t+1,:));
    % (b) resample
    for i = 1:n
        U = rand;
        ind = find(U-Wh(t+1,:)<0);
        x(:,i,t+1) = xh(:,ind(1),t+1);
    end
    xx(:,t+1) = xh(:,:,t+1)*wh(t+1,:)';
end
xh3=squeeze(xx);
x3=x3(:,1:t+1);
R2msmc  =1-sum((x3-xh3).^2,2)./sum((x3-mean(x3,2)*ones(1,M)).^2,2)

toc


% figure(3)
% hold on;
% tl = ['px','py','vx','vy'];
% for i=1:4
%     subplot(4,1,i)
%     plot(1:t,x3(i,1:t), 1:t,xx(i,1:t));
%     xlabel('time')
%     %ylim([-1e2, 1e2])
%     xlim([0,910])
%     ylabel(tl(2*(i-1)+1:2*(i-1)+2),'fontsize',12)
%     legend('True Data','Kalman Filter Estimation')
% end

figure(4)
nn=[20 50 100 500];
R1=[.3157 .7465 .3986 .6474]';
R2=[.3826 .7449 .4581 .6938]';
R3=[.4323 .7865 .5039 .7139]';
R4=[.4598 .7968 .5369 .7346]';
RR=[R1 R2 R3 R4]';
plot(nn, RR)
legend('Px','Py','Vx','Vy')

