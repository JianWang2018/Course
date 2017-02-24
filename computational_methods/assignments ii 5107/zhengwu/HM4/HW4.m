clear; close all;

N = 20;
X = sign(randn(N,N));
XX = X;

H = 1;
J = 0;

%define the neighbor
for i = 1:N
    for j = 1:N
        temp = [i-1,j; i+1,j; i,j-1; i,j+1];
        ngh{i,j} = temp(min(temp, [], 2) >= 1 & max(temp, [], 2) <= N, :);
    end
end

%K sweeps
K = 20;
for k = 2:K
    for i = 1:N
        for j=1:N
            sn = H;
            for r=1:length(ngh(i,j))
                sn = sn+ J*X(ngh{i,j}(r,1), ngh{i,j}(r,2));
            end
            p1 = exp(sn*2)/(1+exp(2*sn));
            U =rand;
            X(i,j) = (U<p1) - (U>p1);
        end
    end
    XX(:,:,k) = X;
end

for k=1:K
    imagesc(XX(:,:,k));
    colormap(gray);
    title(sprintf('%d-th run', k));
    pause; clf;
end
                