#include "solver.h"

vector<Point> permutate(Instance &instance, vector<int> permutations);

vector<int> exchangePositions(vector<int> originalVec, int valueToExchange, int otherValue);

int findPointIn(Point &point, Instance &instance);

bool visited(int point, unsigned long long int mask);

unsigned long long int visit(unsigned long long int mask, int point);

vector<vector<vector<int>>> matrix;
// bi dimensional -> x = ponto atual, y = mascara

void initMatrix(int n) {
    matrix = vector<vector<vector<int>>>(n);

    for (int i = 0; i < n; ++i) {
        unsigned long long possible_masks = (1<<n);
        matrix[i] = vector<vector<int>>(possible_masks);
        for (int j = 0; j < possible_masks; ++j) {
            matrix[i][j] = vector<int>(0);
        }
    }
}

double getDistance(Point p1, Point p2){
	double delta_x = p1.x - p2.x;
	double delta_y = p1.y - p2.y;

	return sqrt(delta_x * delta_x + delta_y * delta_y); 
}

double getFullCost(vector<int> points, Instance &instance) {
    double sum = 0;
    for (int i = 1; i < points.size(); ++i) {
        sum += getDistance(instance.points[points.at(i)], instance.points[points.at(i-1)]);
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

bool allVisited(unsigned long long mask, int n) {
    return mask == ((1<<n) - 1);
}

bool visited(int point, unsigned long long mask) {
    return (mask & (1 << point)) > 0;
}

unsigned long long visit(unsigned long long mask, int point) {
    return mask | (1 << point);
}


vector<int> solveTopDownWithMask(
        Instance &instance, unsigned long long mask, int current_pt,
        int timelimit, chrono::high_resolution_clock::time_point &started
        ) {
    // get the time that has passed in seconds and check the timelimit
//    auto done = chrono::high_resolution_clock::now();
//    auto time = chrono::duration_cast<chrono::seconds>(done - started).count();

//    if (time > timelimit) {
//        return vector<int>(0);
//    }

    if (!matrix[current_pt][mask].empty()) {
        return matrix[current_pt][mask];
    }

    if (allVisited(mask, instance.n - 1)) {
        vector<int> res;
        res.push_back(instance.n - 1);
        res.push_back(current_pt);
        matrix[current_pt][mask] = res;
        return res;
    }

    vector<int> best_sol;
    double best_sol_cost = -1;

    for (int point = 1; point < instance.n - 1; ++point) {
        if (visited(point, mask)) {
            continue;
        }

        unsigned long long newMask = visit(mask, point);
        vector<int> current_sol = solveTopDownWithMask(instance, newMask, point, timelimit, started);
        current_sol.push_back(current_pt);
        double current_sol_cost = getFullCost(current_sol, instance);

        if (best_sol_cost == -1 || best_sol_cost > current_sol_cost) {
            best_sol_cost = current_sol_cost;
            best_sol = current_sol;
        }
    }

    matrix[current_pt][mask] = best_sol;
    return best_sol;
}

vector<int> solveTopDown(Instance &instance, int timelimit, chrono::high_resolution_clock::time_point &started){
    unsigned long long mask = 1;
    // mask = mascara com n bits. se o bit i está ocupado o ponto i ja foi visitado na recursão em questão

    initMatrix(instance.n - 1);
    vector<int> res = solveTopDownWithMask(instance, mask, 0, timelimit, started);

    std::reverse(res.begin(), res.end()); // nossa funcao retorna com n na primeira posicao

    return res;
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