%% ============= Shooting angle =============

data = csvread('/Users/karadza3a/Desktop/data.txt');
X = data(:, 1:4);
y = data(:, 5);
m = length(y);

initial_theta = [10;10];

[theta, J, ~] = ...
	fminsearch(@(t)(vectorAngleCost(t, X, y)), initial_theta);

fprintf('Theta cost: %f \n', J);
[~, hAngle] = vectorAngleCost(theta, X, y);

plot(sort(abs(hAngle - y)));

%% ============= Bounce angle =============

data = csvread('/Users/karadza3a/Desktop/data.txt');

X = [cos(hAngle) sin(hAngle)];
y = data(:, 6);
m = length(y);

initial_theta = -1;

[theta, J, ~] = ...
	fminsearch(@(t)(vectorBounceAngleCost(t, X, y)), ...
        initial_theta, options);

fprintf('Theta cost: %f \n', J);
[~, h] = vectorBounceAngleCost(theta, X, y);

plot(sort(abs(h-y)));
