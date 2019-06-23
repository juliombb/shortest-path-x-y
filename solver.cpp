#include "solver.h"

vector<Point> permutate(Instance &instance, vector<int> permutations);

vector<int> exchangePositions(vector<int> originalVec, int valueToExchange, int otherValue);

int findPointIn(Point &point, Instance &instance);

short initiated = false;
vector<int>** matrix;
// bi dimensional -> x = final, y = quantidade de pontos -1

void initMatrix(int n) {
    if (initiated) {
        return;
    }

    matrix = (vector<int>**) malloc(n*sizeof(vector<int>*));

    for (int i = 0; i < n; ++i) {
        matrix[i] = (vector<int>*) malloc(n*sizeof(vector<int>));
        for (int j = 0; j < n; ++j) {
            matrix[i][j].push_back(-1);
        }
    }
    initiated = true;
}

double getDistance(Point p1, Point p2){
	double delta_x = p1.x - p2.x;
	double delta_y = p1.y - p2.y;

	return sqrt(delta_x * delta_x + delta_y * delta_y); 
}

double getFullCost(vector<Point> points) {
    double sum = 0;
    for (int i = 1; i < points.size(); ++i) {
        sum += getDistance(points[i], points[i-1]);
    }

    return sum;
}

vector<int> copyVec(const vector<int>& other) {
    vector<int> newOne;
    for (int i : other) {
        newOne.push_back(i);
    }

    return newOne;
}

vector<Point> copyVec(const vector<Point>& other) {
    vector<Point> newOne;
    for (Point i : other) {
        newOne.push_back(i);
    }

    return newOne;
}

int findPointIn(Point &point, Instance &instance) {
    for (int i = 0; i < instance.n; ++i) {
        if (instance.points[i].x == point.x && instance.points[i].y == point.y) {
            return i;
        }
    }

    return -1; // should never happen
}

vector<Point> permutate(Instance &instance, vector<int> permutations) {
    vector<Point> points = copyVec(instance.points);
    for (int i = 0; i < permutations.size(); ++i) {
        points[i] = instance.points[permutations[i]];
    }
    return points;
}

vector<int> solveBottomUp(Instance &instance, int timelimit, chrono::high_resolution_clock::time_point &started){
	vector<int> sol;
	
	for (int i = 1; i < instance.n - 1; i++){
		sol.push_back(i);
		
		// get the time that has passed in seconds and check the timelimit
		auto done = chrono::high_resolution_clock::now();
		auto time = chrono::duration_cast<chrono::seconds>(done-started).count();
		
		if (time > timelimit){
			break;
		}
	}
	
	return sol;
}

vector<int> solveTopDown(Instance &instance, int timelimit, chrono::high_resolution_clock::time_point &started) {
    // get the time that has passed in seconds and check the timelimit
    auto done = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::seconds>(done - started).count();

//    if (time > timelimit) {
//        return vector<int>(0);
//    }

    if (instance.n <= 2) {
        vector<int> v(2, 1);
        v[0] = 0;
        return v;
    } else if (instance.n <= 3) {
        vector<int> v(3, 2);
        v[0] = 0;
        v[1] = 1;
        return v;
    }

//    initMatrix(instance.n);
//
//    int last_point_index = findPointIn(instance.points[instance.n - 1], original_instance);
//    if (last_point_index < 0) {
//        return vector<int>(0);
//    }
//    if (matrix[last_point_index][instance.n - 1][0] != -1) {
//        return matrix[last_point_index][instance.n - 1];
//    }

    vector<int> best_sol;
    double best_sol_cost = -1;

    for (
            int permutation_with_prev_last = 1;
            permutation_with_prev_last < instance.n - 1;
            ++permutation_with_prev_last
            ) {
        // calculamos todas as iteracoes possiveis de n-1
        // sendo sempre o primeiro igual e o ultimo diferente

        vector<Point> newPoints = copyVec(instance.points);
        newPoints.pop_back();
        Instance current_instance;
        current_instance.points = newPoints;
        current_instance.instanceName = instance.instanceName;
        current_instance.n = instance.n - 1;

        Point prev_last = instance.points[instance.n - 2];
        Point current_last = instance.points[permutation_with_prev_last];

        current_instance.points[permutation_with_prev_last] = prev_last;
        current_instance.points[current_instance.n - 1] = current_last;

        vector<int> current_sol_permutations;

        current_sol_permutations = solveTopDown(current_instance, timelimit, started);

        if (current_sol_permutations.empty()) {
            return vector<int>(0);
        }

        vector<Point> current_sol = permutate(current_instance, current_sol_permutations);
        double current_sol_cost = getFullCost(current_sol);
        vector<int> current_sol_final;
        current_sol_final = exchangePositions(current_sol_permutations, permutation_with_prev_last, current_instance.n - 1);
        if (best_sol_cost == -1
            || (current_sol_cost + getDistance(current_last, instance.points[instance.n - 1]) < best_sol_cost)) {
            best_sol_cost = current_sol_cost + getDistance(current_last, instance.points[instance.n - 1]);
            best_sol = exchangePositions(current_sol_permutations, permutation_with_prev_last, current_instance.n - 1);
            best_sol.push_back(current_instance.n);
        }
    }

    // matrix[last_point_index][instance.n - 1] = best_sol;
    return best_sol;
}

vector<int> exchangePositions(vector<int> originalVec, int valueToExchange, int otherValue) {
    vector<int> ret;
    for (int i = 0; i < originalVec.size(); ++i) {
        if (originalVec[i] == valueToExchange) {
            ret.push_back(otherValue);
        } else if (originalVec[i] == otherValue) {
            ret.push_back(valueToExchange);
        } else {
            ret.push_back(originalVec[i]);
        }
    }

    return ret;
}