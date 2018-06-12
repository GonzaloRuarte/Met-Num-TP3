function [  ] = pasarDicom_a_pgm( imagefile,outputfile )
%PASARDICOM_A_PGM Summary of this function goes here
%   Detailed explanation goes here
    img = dicomread(imagefile);
    img = uint16(int32(img)+2^15);
    imwrite(img, 'outputfile');

end

