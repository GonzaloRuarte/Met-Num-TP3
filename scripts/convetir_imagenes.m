directorio_entrada = '/home/christian/DICOM/';
directorio_salida  = '/home/christian/PPM/';

convertir_imagenes(directorio_entrada, directorio_salida);

function convertir_imagenes(directorio_entrada, directorio_salida)
    listado_imagenes = dir(directorio_entrada);
    for i=1:length(listado_imagenes)
        if (listado_imagenes(i).name~='.')
           convertir_imagenes_del_directorio(strcat(directorio_entrada, listado_imagenes(i).name, '/'), strcat(directorio_salida, listado_imagenes(i).name, '/'))
        end
    end    
end

function convertir_imagenes_del_directorio(path_entrada, path_salida)
   mkdir(path_salida);
   listado_imagenes = dir(path_entrada);
   for i=1:length(listado_imagenes)
        if ((listado_imagenes(i).name=='.') & (listado_imagenes(i).name=='..'))
        else 
           convertir_imagen(path_entrada, listado_imagenes(i).name, path_salida)
        end
    end
end

function convertir_imagen(path_entrada, nombre_imagen, path_salida)
    [X, map] = dicomread(strcat(path_entrada, nombre_imagen));
    im = im2uint16(X);
    %figure
    %imshow(X,[]);
    %disp(im);
    nombre_salida = strcat(path_salida, nombre_imagen(1:end-4), '.pgm');
    disp(nombre_salida)
    imwrite(im, nombre_salida, 'pgm');
end
