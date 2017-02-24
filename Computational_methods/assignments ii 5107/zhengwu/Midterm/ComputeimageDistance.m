function [D,d0] = ComputeimageDistance(x,y,u,v,b,e,I1,I2,disp)

%first show 2 images;




%compute the log-fourier transfrom;
[xi1,xi2,fh,ft_r,ft_i]=MyLogImageFourierTransform(x,y,I1,u,v)