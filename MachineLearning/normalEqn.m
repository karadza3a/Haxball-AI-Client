%% 
f = 30; % number of features
data = csvread('/Users/karadza3a/Desktop/data.txt');
X = data(:, 1:f);
y = data(:, f+1);
m = length(y);

% Add intercept term to X
X = [ones(m, 1) X];

% Calculate the parameters from the normal equation
theta = pinv(X'*X)*X'*y;

J = sum((X * theta - y).^2) / (2*m);
fprintf('Theta cost: %f \n', J);
fprintf('Plotting error (sorted)\n');

csvwrite('theta.dat',theta');
type theta.dat

plot(sort(abs(X * theta - y)))

% ============================================================