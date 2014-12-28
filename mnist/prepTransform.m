function [mu trmx] = prepTransform(tvec, comp_count)
% tvec - maciery zawieraj¹ca obrazy zbioru ucz¹cego
% comp_count - liczba najwiêkszych sk³adowych w macierzy transformacji
% mu - wartoœæ œrednia zbioru ucz¹cego
% trmx - macierz transformacji do comp_count-wymiarowej przestrzeni PCA
% Funkcja zapisuje w plikach wektor œredniej zbioru ucz¹cego, oraz WSZYSTKIE wektory w³asne 
% w kolejnoœci od odpowiadaj¹cego najwiêkszej wartoœci w³asnej (w tej wersji zapis wykomentowany!)

mu = mean(tvec);
cmx = cov(tvec); 

% Poni¿szy fragment kodu jest liczeniem (d³ugim) macierzy
% kowariancji na piechotê.
%tic;
%cmx = zeros(size(tvec,2));
%f1 = zeros(size(tvec,1), 1);
%f2 = zeros(size(tvec,1), 1);
%for i=1:size(tvec,2)
%  f1(:,1) = tvec(:,i) - repmat(mu(i), size(tvec,1), 1);
%  cmx(i, i) = f1' * f1;
%  for j=i+1:size(tvec,2)
%    f2(:,1) = tvec(:,j) - repmat(mu(j), size(tvec,1), 1);
%    cmx(i, j) = f1' * f2;
%	cmx(j, i) = cmx(i, j);
%  end
%end
%cmx = cmx / (size(tvec,1)-1);
%toc

[evec eval] = eig(cmx);
eval = sum(eval);
[eval evid] = sort(eval);
eval = eval(size(eval,2):-1:1);
evec = evec(:, evid(size(eval,2):-1:1));

% save 'nist_mu.mat' mu
% save 'nist_cov.mat' evec 

trmx = evec(:, 1:comp_count);
