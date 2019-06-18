#ifndef SOLVER_H
#define SOLVER_H
#include <vector>
#include <map>
#include <cmath>
#include "point.h"
#include "instance.h"
#include <bits/stdc++.h> 
#include <algorithm>
#include <chrono>

#define M 32

using namespace std;

double getDistance(Point p1, Point p2);
vector<int> solveBottomUp(Instance &instance, int timelimit, chrono::high_resolution_clock::time_point &started);
vector<int> solveTopDown(Instance &instance, int timelimit, chrono::high_resolution_clock::time_point &started);
#endif // SOLVER_H
