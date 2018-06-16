function [ D_k ] = trazar_recta( punto_inicio, punto_fin, n )
%CREARRECTA trazo la recta poniendo 1's en una matriz de ceros segun si
%pasa en el punto o no, n es el tamaño de la imagen.
%   Detailed explanation goes here
    D_k = zeros(n);
    if(punto_inicio(1) == punto_fin(1))
        D_k(:,punto_inicio(1)) = 1;
        return
    elseif(punto_inicio(1)<punto_fin(1))
        inicio = punto_inicio(1);
        fin = punto_fin(1);
    else
        inicio = punto_fin(1);
        fin = punto_inicio(1);
    end
    punto_inicio = double(punto_inicio);
    punto_fin = double(punto_fin);
    a = (punto_fin(2)-punto_inicio(2))/(punto_fin(1)-punto_inicio(1)); % busco la ecuacion y=ax+b
    b = punto_inicio(2) - a*punto_inicio(1); % b = y - ax
    for i = inicio:fin-1
        desde = floor(a*i+b); % calculo el pixel por el que pasa por encima en el momento i
        hasta = floor(a*(i+1)+b);
        if(hasta<desde)
            aux = hasta
            hasta = desde
            desde = aux
        end
        D_k(desde:hasta,i) = 1; % Agrego todos los pixeles por los que va a pasar hasta llegar a i+1
    end
end

