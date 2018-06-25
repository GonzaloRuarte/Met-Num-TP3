function trazar_recta_real2(pto1, pto2, n)
  if(pto1 == pto2)
    disp('La recta no est´a definida');
    return
  end
  A = zeros(n);
  if(pto1(1) == pto2(1)) %Si los puntos forman una recta vertical...
    if(pto1(1) > 0 & pto1(1) <= n) %en rango...
      A(:, pto1(1)) = ones(n, 1); %pongo unos en la columna correspondiente.
    else
      disp('La recta no pasa por la grilla')
    end
    return  %Termin´e
  end
  a = (pto2(2)-pto1(2))/(pto2(1)-pto1(1));
  if(abs(a) < 100)
    b = pto1(2) - a*pto1(1);  %f(x) = a*x + b
    primera_x = 1;
    ultima_x  = n;
    if(b < 0) %(a*(prim_x-1) + b) < 0
      primera_y = 0;
      primera_x = -b/a; %prim_x = f^-1(primera_y)
    elseif(b > n)
      primera_y = n;
      primera_x = (primera_y-b)/a;  %prim_x = f^-1(primera_y)
    else
      primera_y = b;
    
  else
    a = (pto2(1)-pto1(1))/(pto2(2)-pto1(2));
    b = pto1(1) - a*pto1(2);
    
    
  end
end