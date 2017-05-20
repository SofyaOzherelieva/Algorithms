#include "ExactSolution.h"

CExactSolution::CExactSolution( const vector<vector<float>> matrix_ )
{
	current_lenght = 1;
	number_of_points = matrix_.size();
	matrix = matrix_;
	visited = vector<bool>( number_of_points, false );
	answer = Solution( 0 );
}

float CExactSolution::Solution( int prev )
{
	if( current_lenght == number_of_points )
		return matrix[0][prev];
	visited[prev] = true;
	current_lenght++;
	float sum = INFINITY;
	float temp_sum = 0;
	for( int current = 0; current < number_of_points; ++current ) {
		if( !visited[current] ) {
			temp_sum = Solution( current ) + matrix[current][prev];
			if( sum > temp_sum ) {
				sum = temp_sum;
			}
		}
	}
	current_lenght--;
	visited[prev] = false;
	return sum;
}
