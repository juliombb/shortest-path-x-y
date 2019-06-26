#include "solver.h"

bool visited(int point, unsigned long long int mask);

unsigned long long int visit(unsigned long long int mask, int point);

vector<vector<vector<int>>> matrix;
// bi dimensional -> x = ponto atual, y = mascara

void initMatrix(int n) {
    matrix = vector<vector<vector<int>>>(n);

    for (int i = 0; i < n; ++i) {
        unsigned long long possible_masks = (1 << n);
        matrix[i] = vector<vector<int>>(possible_masks);
        for (int j = 0; j < possible_masks; ++j) {
            matrix[i][j] = vector<int>(0);
        }
    }
}

double getDistance(Point p1, Point p2) {
    double delta_x = p1.x - p2.x;
    double delta_y = p1.y - p2.y;

    return sqrt(delta_x * delta_x + delta_y * delta_y);
}

double getFullCost(vector<int> points, Instance &instance) {
    double sum = 0;
    for (int i = 1; i < points.size(); ++i) {
        sum += getDistance(instance.points[points.at(i)], instance.points[points.at(i - 1)]);
    }

    return sum;
}

bool allVisited(unsigned long long mask, int n) {
    return mask == ((1 << n) - 1);
}

bool visited(int point, unsigned long long mask) {
    return (mask & (1 << point)) > 0;
}

unsigned long long visit(unsigned long long mask, int point) {
    return mask | (1 << point);
}

vector<int> solveBottomUp(Instance &instance, int timelimit, chrono::high_resolution_clock::time_point &started) {

    initMatrix(instance.n - 1);

    for (unsigned long long mask = (1 << (instance.n - 1)) - 1; mask > 0; mask--) {
        // mask = mascara com n bits. se o bit i está ocupado o ponto i ja foi visitado na recursão em questão
        for (int current_pt = 0; current_pt < instance.n - 1; current_pt++) {

            if (!visited(current_pt, mask)) {
                continue;
            }

            auto done = chrono::high_resolution_clock::now();
            auto time = chrono::duration_cast<chrono::seconds>(done - started).count();

            if (time > timelimit) {
                return vector<int>(0);
            }

            if (allVisited(mask, instance.n - 1)) {
                vector<int> res;
                res.push_back(instance.n - 1);
                res.push_back(current_pt);
                matrix[current_pt][mask] = res;
                continue;
            }

            vector<int> best_sol;
            double best_sol_cost = -1;

            for (int point = 1; point < instance.n - 1; ++point) {
                if (visited(point, mask)) {
                    continue;
                }

                unsigned long long newMask = visit(mask, point);
                vector<int> current_sol = matrix[point][newMask];
                current_sol.push_back(current_pt);
                double current_sol_cost = getFullCost(current_sol, instance);

                if (best_sol_cost == -1 || best_sol_cost > current_sol_cost) {
                    best_sol_cost = current_sol_cost;
                    best_sol = current_sol;
                }
            }

            matrix[current_pt][mask] = best_sol;
        }
    }

    vector<int> res = matrix[0][1];

    std::reverse(res.begin(), res.end());
        // nossa funcao retorna com n na primeira posicao

    return res;
}

vector<int> solveTopDownWithMask(
        Instance &instance, unsigned long long mask, int current_pt,
        int timelimit, chrono::high_resolution_clock::time_point &started
) {
    // get the time that has passed in seconds and check the timelimit
    auto done = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::seconds>(done - started).count();

    if (time > timelimit) {
        return vector<int>(0);
    }

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

        if (current_sol.empty()) {
            return vector<int>(0);
        }

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

vector<int> solveTopDown(Instance &instance, int timelimit, chrono::high_resolution_clock::time_point &started) {
    unsigned long long mask = 1;
    // mask = mascara com n bits. se o bit i está ocupado o ponto i ja foi visitado na recursão em questão

    initMatrix(instance.n - 1);
    vector<int> res = solveTopDownWithMask(instance, mask, /*current_pt*/0, timelimit, started);

    std::reverse(res.begin(), res.end()); // nossa funcao retorna com n na primeira posicao

    return res;
}