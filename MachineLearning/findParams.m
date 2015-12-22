%% ============= Part 2: Regularization and Accuracies =============

data = csvread('/Users/karadza3a/Desktop/data.txt');
X = data(:, 1:4);
y = data(:, 5);
m = length(y);

initial_theta = [10;10];

[theta, J, exit_flag] = ...
	fminsearch(@(t)(costVectors(t, X, y)), initial_theta);

plot(sort(abs(h-y)));
