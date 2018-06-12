function [ puntos_inicio, puntos_fin ] = inicios_fines_horizontales( n )
%INICIOS_FINES_HORIZONTALES Dada una imagen de nxn, crea los puntos de
%inicio fin de todas las rectas horizontales posibles
%pensar como coordenadas del espacio xy, no como fila o columna de una matriz.
%   Detailed explanation goes here
    puntos_inicio = zeros(n,2); %n puntos con coordenadas x,y.
    puntos_fin = zeros(n,2);    %idem
    for i = 1:n
        puntos_inicio(i,1) = 1;
        puntos_inicio(i,2) = i;
        puntos_fin(i,1) = n;
        puntos_fin(i,2) = i;
    end

end

