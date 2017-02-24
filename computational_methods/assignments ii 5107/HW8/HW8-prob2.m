clear all; close all;

x = [1 2 3.5 4 7 7.3 8.6 12.4 13.8 18.1];
 
x_sort = sort(x);
 
x_trimmed=x_sort(3:8);
mean_trim = mean(x_sort(3:8));
 
for t=1:50
    
    B = 2000;
    bx = zeros(1,6,B);
 
    for i=1:B
        ind = randsample(6,6,'true');
         bx(1,:,i) = x_trimmed(1,ind);
    end;
 
% trimmed mean using bootstrap;
    for i=1:B
        BST_mean(i) = mean(bx(1,:,i));
    end;
 
    SE_BST_mean(t) = std(BST_mean);
end;

% assess the variability of the stand eror
std(SE_BST_mean)
