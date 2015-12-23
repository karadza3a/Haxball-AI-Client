function [J, h] = vectorBounceAngleCost(theta, X, y)

m = length(y); % number of training examples

%% ==== Calculate h
result = X*[1 0; 0 theta];
h = atan2(result(:, 2),result(:, 1));


%% ==== Calculate J

J = (h-y)'*(h-y); % squared error
J = J/m;

% =============================================================

end
