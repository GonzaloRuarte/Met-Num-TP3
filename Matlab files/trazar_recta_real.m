function A = trazar_recta_real(pto1, pto2, tamanio)
  A = zeros(tamanio);
  if(ceil(pto1(1)) == ceil(pto2(1))) %Si ambos puntos est´an sobre la misma columna...
    if(ceil(pto1(1)) >= 1 & ceil(pto1(1)) <= tamanio) %y en rango...
      A(:, ceil(pto1(1))) = ones(tamanio, 1); %pongo unos en dicha columna.
    else
      disp('La recta no pasa por la grilla')
    endif
  elseif(ceil(pto1(2)) == ceil(pto2(2))) %Si ambos puntos est´an sobre la misma fila...
    if(ceil(pto1(2)) >= 1 & ceil(pto1(2)) <= tamanio) %y en rango...
      A(tamanio+1 - ceil(pto1(2)), :) = ones(1, tamanio); %pongo unos en dicha fila.
    else
      disp('La recta no pasa por la grilla')
    endif
  else
    a = (pto2(2)-pto1(2))/(pto2(1)-pto1(1));
    if(abs(a) < 1) %Si la recta en funci´on de x tiene pendiente menor a 1,la sigo pensando en funci´on de x
      b = pto1(2) - a*pto1(1);
      %input('breakpoint');
      if(pto1(1) < pto2(1))
        primera = ceil(pto1(1)); %Primera l´inea vertical que considero
        ultima  = ceil(pto2(1)); %´Ultima l´inea vertical que considero
      else
        primera = ceil(pto2(1)); %Primera l´inea vertical que considero
        ultima  = ceil(pto1(1)); %´Ultima l´inea vertical que considero
      endif
      if(primera == 0)  %No empiezo desde 0.
        primera = 1;    %Empiezo desde 1.
      endif
      if(a > 0)
        y = a*(primera-1) + b;
        fila_ant = max(ceil(y), 1);
        if(y == fila_ant) %Si empieza en una esquina.
          fila_ant = fila_ant+1
        endif
        for x = primera:ultima-1  %El ´ultimo caso requiere un trato especial
          y = a*x+b;
          fila_act = ceil(y);
          A(tamanio+1 -(fila_ant:fila_act), x) = 1;
          if(y == fila_act) %caso en que pasa por una esquina.
            fila_ant = fila_act+1;
          else
            fila_ant = fila_act;
          endif
        endfor
        fila_act = min(ceil(a*ultima+b), tamanio);
        A(tamanio+1 -(fila_ant:fila_act), ultima) = 1;
      else
        fila_ant = min(ceil(a*(primera-1) + b), tamanio);
        for x = primera:ultima-1  %El ´ultimo caso requiere un trato especial
          y = a*x+b;
          fila_act = ceil(y);
          if(y == fila_act) %caso en que pasa por una esquina.
            A(tamanio+1 -(fila_act+1:fila_ant), x) = 1;
          else
            A(tamanio+1 -(fila_act:fila_ant), x) = 1;
          endif
          fila_ant = fila_act;
        endfor
        y = a*ultima+b;
        fila_act = max(ceil(y), 1);
        if(y == fila_act) %Si termina en una esquina.
          A(tamanio+1 -(fila_act+1:fila_ant), ultima) = 1;
        else
          A(tamanio+1 -(fila_act:fila_ant), ultima) = 1;
        endif
      endif
    else  %Si no, la pienso en funci´on de y.
      a = (pto2(1)-pto1(1))/(pto2(2)-pto1(2));
      b = pto1(1) - a*pto1(2);
      if(pto1(2) < pto2(2))
        primera = ceil(pto1(2)); %Primera l´inea horizontal que considero
        ultima  = ceil(pto2(2)); %´Ultima l´inea horizontal que considero
      else
        primera = ceil(pto2(2)); %Primera l´inea horizontal que considero
        ultima  = ceil(pto1(2)); %´Ultima l´inea horizontal que considero
      endif
      if(primera == 0)  %No empiezo desde 0.
        primera = 1;    %Empiezo desde 1.
      endif
      if(a > 0)
        x = a*(primera-1) + b;
        columna_ant = max(ceil(x), 1);
        if(x == columna_ant)  %Si empieza en una esquina.
          columna_ant = columna_ant+1;
        endif
        for y = primera:ultima-1  %El ´ultimo caso requiere un trato especial
          x = a*y+b;
          columna_act = ceil(x);
          A(tamanio+1 -y, columna_ant:columna_act) = 1;
          if(x == columna_act) %caso en que pasa por una esquina.
            columna_ant = columna_act+1;
          else
            columna_ant = columna_act;
          endif
        endfor
        columna_act = min(ceil(a*ultima+b), tamanio);
        A(tamanio+1 -ultima, columna_ant:columna_act) = 1;
      else
        columna_ant = min(ceil(a*(primera-1) + b), tamanio);
        for y = primera:ultima-1  %El ´ultimo caso requiere un trato especial
          x = a*y+b;
          columna_act = ceil(x);
          if(x == columna_act) %caso en que pasa por una esquina.
            A(tamanio+1 -y, (columna_act+1:columna_ant)) = 1;
          else
            A(tamanio+1 -y, (columna_act:columna_ant)) = 1;
          endif
          columna_ant = columna_act;
        endfor
        x = a*ultima+b;
        columna_act = max(ceil(x), 1);
          if(x == columna_act)  %Si termina en una esquina.
            A(tamanio+1 -ultima, (columna_act+1:columna_ant)) = 1;
          else
            A(tamanio+1 -ultima, (columna_act:columna_ant)) = 1;
          endif   
      endif
    endif
  endif
  figure
  plot([pto1(1), pto2(1)], [pto1(2), pto2(2)], 6);
  xlim([0, tamanio]);
  ylim([0, tamanio]);
endfunction