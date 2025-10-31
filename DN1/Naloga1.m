fprintf('\nBranje datoteke')

filename= 'naloga1_1.txt';

delimiterIn= ' ';
headelinesIn= 2;

podatki = importdata(filename,delimiterIn, headelinesIn);

t = podatki.data(:,1);

fprintf('Prebranih vrednosti: %d\n', numel(t));
disp(t(1:5));
disp(t(end-4:end));

%rezultati na grafu

figure(1); clf;
plot(t, 'o-'); grid on;

% 2.del naloge

filenameP = 'naloga1_2.txt';

fid = fopen(filenameP, 'r');

firstLine = fgetl(fid);
N = sscanf(firstLine,'%*[^0-9]%d', 1); %odstranimo tekst iz vrednosti

P = zeros(N,1);
for i = 1:N
    line = fgetl(fid);
    P(i) = str2double(line);
end

fclose(fid);

%izris na grafu

figure(3);
plot(t, P, 'o-');
grid on;
xlabel('t[s]');
ylabel('P[W]');
title('graf P(t)');


% 3.del naloge- Integral

% ročno računanje integrala

M= min(numel(t), numel(P));

tI = t(1:M);
PI = P(1:M);


E_for = 0;
for k = 1:(M-1)
    dt = tI(k+1) - tI(k);
    avgP = 0.5 * (PI(k)+ PI(k+1));
    E_for = E_for + avgP * dt;
end

fprintf('E (for-zanka) = %.6f J\n', E_for);

%Preverimo resitev se s funkcijo trapz

E_trapz = trapz(tI, PI);
fprintf('E (trapz) = %.6f J\n', E_trapz);






