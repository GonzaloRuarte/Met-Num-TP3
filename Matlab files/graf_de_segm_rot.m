function graf_de_segm_rot(n)
  x1s = 0:n;
  x2s = x1s;
  y1s = zeros(1, n+1);
  y2s = ones(1, n+1)*n;
  plot([x1s; x2s], [y1s; y2s], 'k');
  hold on;
  plot([y1s; y2s], [x1s; x2s], 'k');
  hold on;
  
  x1s = 0.5:(n-0.5);
  x2s = x1s;
  y1s = zeros(1, n);
  y2s = ones(1, n)*n;
  plot([x1s; x2s], [y1s; y2s], 'b');
  for i = 1:n
    input('breakpoint')
    x1s(1:n-i) = x1s(1:n-i) .+1;
    x1s(n-i+1) = n;
    x2s(i+1:n) = x2s(i+1:n) .-1;
    x2s(i) = 0;
    y1s(n-i+1) = 0.5;
    y1s(n-i+2:n) = y1s(n-i+2:n) .+1;
    y2s(i) = n-0.5;
    y2s(1:i-1) = y2s(1:i-1) .-1;
    hold on;
    %plot([x1s; x2s], [y1s; y2s], int2str(i));
    plot([x1s; x2s], [y1s; y2s], 'b');
  endfor
  
end