function [  ] = pasarPNG_a_pgm( imagefile,outputfile )
%PASARPNG_A_PGM lo hace transformandolo a 16 bits (obviamente png usa 8
%bits por lo que simplemente multiplico por 256 para que tengamos algo de
%16 bits (no ideal pero no podemos crear informacion)
%   Detailed explanation goes here
    A = imread(imagefile);
    A = A(:,:,1);
    A = uint16(A)*256 + 128;
    imwrite(A,outputfile);

end

