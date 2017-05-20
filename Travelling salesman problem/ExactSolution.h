#pragma once

#include "Graph.h"


class CExactSolution {
public:
	CExactSolution( const vector<vector<float>> matrix_ );
	float Solution( int pos );
	float answer;
private:
	int number_of_points;
	int current_lenght;
	vector<vector<float>> matrix;
	vector<bool> visited;
};
