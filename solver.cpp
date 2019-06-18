#include "solver.h"

vector<Point> permutate(Instance &instance, vector<int> permutations);

short initiated = false;
vector<int>** matrix;
// bi dimensional -> x = final, y = quantidade de pontos

void initMatrix(int n) {
    if (initiated) {
        return;
    }

    matrix = (vector<int>**) malloc(n*sizeof(vector<int>*));

    for (int i = 0; i < n; ++i) {
        matrix[i] = (vector<int>*) malloc(n*sizeof(vector<int>));
        for (int j = 0; j < n; ++j) {
            matrix[i][j][0] = -1;
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

vector<Point> copyVec(const vector<Point>& other) {
    vector<Point> newOne(other.size());
    for (Point i : other) {
        newOne.push_back(i);
    }

    return newOne;
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

vector<int> solveTopDown(Instance &instance, int timelimit, chrono::high_resolution_clock::time_point &started){
    // get the time that has passed in seconds and check the timelimit
    auto done = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::seconds>(done-started).count();

    if (time > timelimit) {
        return vector<int>(0);
    }

    if (instance.n <= 2) {
        vector<int> v(2, 1);
        v[0] = 0;
        return v;
    } else if (instance.n <= 3) {
        vector<int> v(3, 2);
        v[0] = 0;
        v[1] = 0;
        return v;
    }

    initMatrix(instance.n);

    vector<int> best_sol;
    double best_sol_cost = -1;

    for (int i = 1; i < instance.n; ++i) {
        // calculamos todas as iteracoes possiveis de n-1
        // sendo sempre o primeiro igual e o ultimo diferente

        vector<Point> newPoints = copyVec(instance.points);
        newPoints.pop_back();
        Instance current_instance;
        current_instance.points = newPoints;
        current_instance.instanceName = instance.instanceName;
        current_instance.n = instance.n - 1;

        Point last = instance.points[i];
        Point prev_last = instance.points[instance.n - 1];

        current_instance.points[i] = prev_last;
        current_instance.points[current_instance.n - 1] = last;

        vector<int> current_sol_permutations;

        if (matrix[i][current_instance.n - 1][0] != -1) {
            current_sol_permutations = matrix[i][current_instance.n - 1];
        } else {
            current_sol_permutations = solveTopDown(current_instance, timelimit, started);
            matrix[i][current_instance.n - 1] = current_sol_permutations;
        }

        vector<Point> current_sol = permutate(instance, current_sol_permutations);
        double current_sol_cost = getFullCost(current_sol);
        if (current_sol_cost + getDistance(last, prev_last) < best_sol_cost) {
            best_sol_cost = current_sol_cost;
            best_sol = current_sol_permutations;
        }
    }

	return best_sol;
}