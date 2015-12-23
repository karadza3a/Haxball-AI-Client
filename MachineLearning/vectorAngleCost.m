function [J, h] = vectorAngleCost(theta, X, y)

m = length(y); % number of training examples

%% ==== Calculate h

v1 = X(:, 1:2); % shot vector [x y]
v2 = X(:, 3:4); % ball velocity vector [x y]

% scale with theta
v1 = (theta(1))*v1;
v2 = (theta(2))*v2;

result = v1+v2;

h = atan2(result(:, 2),result(:, 1));

%% ==== Calculate J

J = (h-y)'*(h-y); % squared error
J = J/m;

% =============================================================

end
