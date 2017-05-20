#include "Graph.h"

#include <iostream>

using std::cout;

CGraph::CGraph( int number_of_points_ )
{
	CGenerator generate( number_of_points_ );
	rand_points = generate.Return_rand_points();

	matrix_of_distances.resize( number_of_points_ );
	for( int i = 0; i < number_of_points_; i++ ) {
		matrix_of_distances[i].resize( number_of_points_ );
		for( int j = 0; j < number_of_points_; j++ ) {
			matrix_of_distances[i][j] = 0;
		}
	}
	number_of_points = number_of_points_;
}

vector<vector<float>> CGraph::Generate_matrix()
{
	for( int i = 0; i < number_of_points; ++i ) {
		for( int j = i + 1; j < number_of_points; ++j ) {
			matrix_of_distances[i][j] = sqrt( pow( ( rand_points[i].first - rand_points[j].first ), 2 ) + pow( ( rand_points[i].second - rand_points[j].second ), 2 ) );
		}
	}
	for( int i = 0; i < number_of_points; i++ ) {
		for( int j = 0; j < i; ++j ) {
			matrix_of_distances[i][j] = matrix_of_distances[j][i];
		}
	}
	//show( matrix_of_distances );
	return matrix_of_distances;
}