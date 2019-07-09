Polygon = [  
     0     0
    10     0
    10    10
     0    10
];


Polygon2 = [
   59.9798  310.1933
   15.6250  491.2127
   16.6331  515.8172
   29.7379  565.0264
   66.0282  654.6573
   87.1976  688.0492
  177.9234  830.4042
  233.3669  895.4306
  248.4879  905.9754
  320.0605  942.8822
  405.7460  974.5167
  521.6734  993.8489
  618.4476  949.9121
  660.7863  927.0650
  681.9556  913.0053
  866.4315  688.0492
  929.9395  591.3884
  929.9395  498.2425
  912.8024  392.7944
  887.6008  311.9508
  832.1573  192.4429
  732.3589   72.9350
  690.0202   50.0879
  630.5444   21.9684
  597.2782   11.4236
  524.6976   16.6960
  482.3589   21.9684
  456.1492   27.2408
  424.8992   34.2707
  346.2702   55.3603
  295.8669   74.6924
  239.4153   97.5395
  179.9395  127.4165
  128.5282  167.8383
  103.3266  206.5026
   85.1815  243.4095
   67.0363  283.8313
   59.9798  310.1933
];
  
A = [];
b = [];



[A, b] = get_A_and_b_for_polygon(Polygon);

% maximize r (== minimize -r)
f = [0 0 -1];

linprog(f,A,b)


[A, b] = get_A_and_b_for_polygon(Polygon2);

f = [0 0 -1];
x = linprog(f, A, b)


X = reshape(Polygon2(:,1),2,[]);
X = X(:);
Y = reshape(Polygon2(:,2),2,[]);
Y = Y(:);
plot(X,Y);
plot_circle(x(1), x(2), x(3));

function plot_circle(x,y,r)
    hold on
    th = 0:pi/50:2*pi;
    xunit = r * cos(th) + x;
    yunit = r * sin(th) + y;
    plot(xunit, yunit);
    hold off
end

function [A, b] = get_A_and_b_for_polygon(polygon)
    A = [];
    b = [];
    for i=1:size(polygon,1)

        i2 = mod(i, size(polygon, 1)) + 1;
        
        if (i == size(polygon, 1))
            if (polygon(i, 1) == polygon(i2, 1) && polygon(i, 2) == polygon(i2, 2))
                % Skip if polygon is cyclic
                continue; 
            end
        end

        % Aufpunkt ist Punkt 1
        aufpunkt = [
            polygon(i, 1)
            polygon(i, 2)
        ];
        % Richtung ist Punkt 2 - Punkt 1
        richtung = [
            (polygon(i2, 1) - aufpunkt(1))
            (polygon(i2, 2) - aufpunkt(2))
        ];

        % Normalenvektor berechnen
        unnormalized_n = [ richtung(2) -richtung(1)];
        n = unnormalized_n./norm(unnormalized_n);

        
        c = n * aufpunkt;

        A(i, 1) = -n(1);
        A(i, 2) = -n(2);
        A(i, 3) = 1;

        b(i) = -c;
       

    end
end