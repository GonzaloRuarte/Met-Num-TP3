function A = trazar_recta_real(pto1, pto2, tamanio)
  A = zeros(tamanio);
  if(ceil(pto1(1)) == ceil(pto2(1))) %Si ambos puntos est´an sobre la misma columna...
    if(ceil(pto1(1)) >= 1 & ceil(pto1(1)) <= tamanio) %y en rango...
      A(:, ceil(pto1(1))) = ones(tamanio, 1); %pongo unos en dicha columna.
    else
      disp('La recta no pasa por la grilla')
    end
    return  %Termin´e
  elseif(ceil(pto1(2)) == ceil(pto2(2))) %Si ambos puntos est´an sobre la misma fila...
    if(ceil(pto1(2)) >= 1 & ceil(pto1(2)) <= tamanio) %y en rango...
      A(tamanio+1 - ceil(pto1(2)), :) = ones(1, tamanio); %pongo unos en dicha fila.
    else
      disp('La recta no pasa por la grilla')
    end
    return  %Termin´e
  end
  a = (pto2(2)-pto1(2))/(pto2(1)-pto1(1));
  b = pto1(2) - a*pto1(1);
  %input('breakpoint');
  if(pto1(1) < pto2(1))
    primera = max(ceil(pto1(1)), 1); %Primera l´inea horizontal que considero
    ultima  = min(ceil(pto2(1)), tamanio); %´Ultima l´inea horizontal que considero
  else
    primera = max(ceil(pto2(1)), 1); %Primera l´inea horizontal que considero
    ultima  = min(ceil(pto1(1)), tamanio); %´Ultima l´inea horizontal que considero
  end
  %input('breakpoint');
  if(a > 0)
    fila_ant = max(ceil(a*(primera-1) + b), 1);
    for x = primera:ultima-1  %El ´ultimo caso requiere un trato especial
      y = a*x+b;
      fila_act = ceil(y);
      A(tamanio+1 -(fila_ant:fila_act), x) = 1;
      %input('breakpoint');
      if(y == fila_act) %caso en que pasa por una esquina.
        fila_ant = fila_act+1;
      else
        fila_ant = fila_act;
      end
    end
    fila_act = min(ceil(a*ultima+b), tamanio);
    A(tamanio+1 -(fila_ant:fila_act), ultima) = 1;
  else
    fila_ant = min(ceil(a*(primera-1) + b), tamanio)
    for x = primera:ultima-1  %El ´ultimo caso requiere un trato especial
      y = a*x+b;
      x
      fila_act = ceil(y)
      if(y == fila_act) %caso en que pasa por una esquina.
        A(tamanio+1 -(fila_act+1:fila_ant), x) = 1
      else
        A(tamanio+1 -(fila_act:fila_ant), x) = 1
      %input('breakpoint');
      end
      fila_ant = fila_act
    end
    fila_act = max(ceil(a*ultima+b), 1)
    A(tamanio+1 -(fila_act:fila_ant), ultima) = 1
    
  end
  figure
  plot([pto1(1), pto2(1)], [pto1(2), pto2(2)], 6);
  xlim([0, tamanio]);
  ylim([0, tamanio]);
end