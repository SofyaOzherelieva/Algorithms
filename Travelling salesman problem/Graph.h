#pragma once
#include "TestGenerator.h"

void show( vector<vector<float>>& matrix );

class CGraph {
public:
	CGraph( int number_of_points );
	vector<vector<float>> Generate_matrix();
private:
	int number_of_points;
	vector<pair<float, float>> rand_points;
	vector<vector<float>> matrix_of_distances;
};
