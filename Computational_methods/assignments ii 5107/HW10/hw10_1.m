% hw 10_1

%% fit the model using training data
clear;

load hw10_1_train;

[T, C] = size(rate);

% choose either x or y position
d = 2;               % 1: x-pos, 2: y-pos
kin = kin(:,d);   

% discretize the kinematics
N = 20;                 % discretization number 
max_kin = max(kin); 
min_kin = min(kin);
nkin = (kin-min_kin)/(max_kin-min_kin);  % scale to [0 1]
dkin = round((N-1)*nkin) + 1;     % discrete state:  1, 2,..., N

% the prior of kinematics p(x_k)
for i = 1:N 
    Px(i) = sum(dkin == i);
end
Px = Px./sum(Px);

% the transition probability p(x_{k+1} | x_k)
for i = 1:N
    for j = 1:N
        Ptx(i,j) = sum(dkin(1:T-1) == i & dkin(2:T) == j);
    end
end
Ptx = Ptx./(sum(Ptx,2)*ones(1,N));

R = max(rate(:))+1;   % firing rate is in {0, 1, 2, ..., R-1}

% the prior of firing rate p(z_k)
for j = 1:R
    Pz(j,:) = sum(rate == j-1);
end
Pz = Pz./(ones(R,1)*sum(Pz));

% the observation probability p(z_k | x_k) for each neuron
for k = 1:C
    for i = 1:N
        for j = 1:R
            Pzx(i,j,k) = sum(dkin == i & rate(:,k) == j-1);
        end
    end
    Pzx(:,:,k) = Pzx(:,:,k)./(sum(Pzx(:,:,k),2)*ones(1,R)+eps);
end

%save hmm_1d Px Ptx Pz Pzx max_kin min_kin d N;

%% decode the hmm in the test data

% use log of probability in the calculation

load hw10_1_test;    % load test data

kin = kin(:,d);   

[T, C] = size(rate);

% joint probability of firing rates of all neurons conditioned on kineamtics 
% assuming independence over neurons
lpz_jx = zeros(T,N);
for i = 1:T
    for k = 1:C
        lpzx(:,k) = log(Pzx(:,rate(i,k)+1,k)+eps);
    end
    lpz_jx(i,:) = sum(lpzx,2)';
end

%%% Dynamic programming to find the optimal kinematics %%%
joint_prob = zeros(T,N);              % delta_t(n) in the notes
joint_prob(1,:) = log(Px+eps)+lpz_jx(1,:);  % delta_1(n)

for k = 2:T
    for m = 1:N
        temp = joint_prob(k-1,:)+log(Ptx(:,m)'+eps)+lpz_jx(k,m);
        [joint_prob(k,m), ind(k,m)] = max(temp);  % ind save optimal pre-index
    end
end

[ignore, x_h(T)] = max(joint_prob(T,:));  % identify last time step

for k = T-1:-1:1                              % recursively identification
    x_h(k) = ind(k+1,x_h(k+1));
end

kin_h = (max_kin-min_kin)*(x_h'-1)/(N-1)+min_kin;   % return to the original scale

subplot(211)
plot(1:T, kin, 1:T, kin_h, 'r');

R2 = 1 - sum((kin-kin_h).^2)/(sum((kin-mean(kin)).^2)),
