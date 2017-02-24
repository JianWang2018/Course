

    
disp = 1;
e = 0;
b = 6;


%I1 = imread('ImageData/Caltech_background/0435.jpg','jpg');


%I2 = imread('ImageData/Caltech_background/0475.jpg','jpg');

I1 = double(XX(:,:,1));
I2 = double(XX(:,:,1));

F1 = fft2(I1);
F2 = fft2(I2);


nx = size(F1, 2);
ny = size(F1, 1);


cxrange = [0:nx/2, -nx/2+1:-1];        % cycles across the image
cyrange = [0:ny/2, -ny/2+1:-1];
[cx, cy] = meshgrid(cxrange, cyrange);

fxrange = cxrange * 2*pi/nx;           % radians per pixel
fyrange = cyrange * 2*pi/ny;
[fx, fy] = meshgrid(fxrange, fyrange);


% % 
% % %blur the two images;
% % 
% %     
%  sigma0 = blur*0.002        % Width of the Gaussian, in radians/pixel
% % 
%  g_blur0 = exp(-(fx.^2 + fy.^2)*(pi*sigma0));
% % 
% % 
% % %..........do bluring....................................
%  smoothF1 = F1 .* g_blur0;          % multiply FT by mask
% % 
% % F1 = smoothF1;
% % %plot the lFT after smoothing;
%  smooth1 = ifft2(smoothF1);    % do inverse FT
% % 
%  I1 = smooth1;
% % 
% % %for the 2-nd image
%  smoothF2 = F2 .* g_blur0;          % multiply FT by mask
% % 
%  F2 = smoothF2;
% % %plot the lFT after smoothing;
%  smooth2 = ifft2(smoothF2);    % do inverse FT
% % 
%  I2 = smooth2;
% % 
% % %..........finish bluring....................................


%show im;

if disp
figure(1);
imagesc(I1),colormap(gray);
figure(2);
imagesc(I2),colormap(gray);
end;


%take the FT;
% F1 = fft2(I1);
% F2 = fft2(I2);
%F1 = smoothF1;
%F2 = smoothF2;

if (disp)
figure(3);
subplot(1,2,1);
imagesc(log(abs(F1))),colormap(gray);
subplot(1,2,2);
imagesc(log(abs(F2))),colormap(gray);
end



% partF = ifft2(F);
% 
% figure(3),imagesc(partF);colormap(gray);


% 
% figure;
% cycles = [0 1 2 4 8];
% n = length(cycles);
% for p = 1:n
%     for q = 1:n
%        keep = abs(cx) <= cycles(p) & abs(cy) <= cycles(q);
%        partF = F .* keep;
%        partim = ifft2(partF);
%        subplot(n, n, n*(p-1) + q);
%        imagesc(partim);colormap(gray);
%     end
% end


% %blure with Gaussian
% 
% %blur with sd=sigma;
% sigma = 1;        % Width of the Gaussian, in radians/pixel
% 
% g_blur = exp(-(fx.^2 + fy.^2)*(pi*sigma^2));
% figure(3);
% imagesc(log(g_blur));colormap(gray);
% 
% %..........do bluring..............
% smoothF = F .* g_blur;          % multiply FT by mask
% 
% %plot the lFT after smoothing;
% figure(4); imagesc(log(abs(smoothF)));colormap(gray);
% 
% smooth = ifft2(smoothF);    % do inverse FT
% figure(5);
% imagesc(smooth);colormap(gray);

log_F1 = log(F1);
log_F2 = log(F2);

Q1 = ComputeImageQDirect(fx,fy,log_F1,b,e)
Q2 = ComputeImageQDirect(fx,fy,log_F2,b,e)
Q0 = ComputeImageQDirect(fx,fy,-pi*(fx.^2+fy.^2),b,e)

[tmp,id] = min([Q1,Q2]);
[tmpm,idm] = max([Q1,Q2]);
c = max(Q1,Q2);
d0 = (c-tmp)/Q0


%then blur the one with small Q;
sigma = d0;
g_blur = exp(-(fx.^2 + fy.^2)*(pi*sigma));

str = sprintf('smoothF = F%d .* g_blur;',id);
eval(str); 

if disp
figure(4); imagesc(log(abs(smoothF)));colormap(gray);
end 

smoothi = ifft2(smoothF);

if disp
figure(5); imagesc(smoothi);colormap(gray);
end;



%compute the geodesic;
if(disp)
    stp = 5;
    figure(99); axes('FontSize',20); clf; hold on;
    for r=1:stp+1
        tau = (r-1)/stp;

        if(tau ~= 0 && tau ~= 1)
        Pftim = tau*double(F1) + (1-tau)*double(F2);

        Pim = ifft2(Pftim);
        
        elseif(tau==0)
                Pim = ifft2(F2);
        elseif(tau==1)
                Pim = ifft2(F1);
        end;
            
        
        subplot(2,3,r);
        %norm(imag(fr))
    
        %figure(99+r); clf; axes('FontSize',20);
        %figure(60+r);
        
        %axis square;
        %M(r) = getframe;
        imagesc(Pim);colormap(gray);
        axis off;
        title(sprintf('%d-th run', r));
        %pause;
    end
end

str=sprintf('d_real = log(abs((smoothF))) - log(abs((F%d)));',idm);
eval(str)


D = InnProdPolyImage(fx,fy,d_real,d_real,b)

% Frobenious norm

F = sqrt(sum(sum((I1-I2).^2,1)));
% 
% end;
% 
% figure(1),
% plot(0.002:0.002:0.4,D);
% figure(2),
% plot(0.002:0.002:0.4,F,'--r');
