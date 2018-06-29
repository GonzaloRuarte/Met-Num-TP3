function [ x ] = a1024( matrix )
%A1024 Summary of this function goes here
%   Detailed explanation goes here
    x = matrix;
    [filas,columnas] = size(matrix);
    filasQueFaltan = 1024 - filas;
    columnasQueFaltan = 1024 - columnas;
    negroFilas = zeros(filas,1);
    negroColumnas = zeros(1,1024);
    negroFilas = negroFilas + 128;
    negroColumnas =  negroColumnas + 128;
    if (mod(filasQueFaltan,2) == 1)
        x = [x,negroFilas];
    end
    for i = 1:columnasQueFaltan/2
        x = [negroFilas,x,negroFilas];
    end
    
    if (mod(columnasQueFaltan,2) == 1)
        x = [x;negroColumnas];
    end
    for i = 1:filasQueFaltan/2
        x = [negroColumnas;x;negroColumnas];
    end
end

