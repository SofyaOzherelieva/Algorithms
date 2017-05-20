#pragma once

#include "Graph.h"

struct edge {
	int from;
	int to;
	float weight;
	edge() {}
	edge( int from, int to, float weight ) : from( from ), to( to ), weight( weight ) {}
};

class CApproximateSolution {
public:
	CApproximateSolution( const vector<vector<float>> matrix_ );
	void FindMST();
	void Find_Eileur_path( int current, vector<vector<float>>& matrix_for_del );
	void Find_result_path();
	void Solution();

	float answer;
private:
	void transform_from_matrixGraph_to_setGraph();

	int number_of_points;
	vector<int> eileurPath;
	vector<int> resultPath;
	vector<vector<float>> MST;
	vector<vector<float>> matrix;
	vector<edge> setGraph;
};