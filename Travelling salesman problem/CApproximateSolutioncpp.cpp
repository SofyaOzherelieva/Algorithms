#include "ApproximateSolution.h"
#include "KruskalAlgorithm.h"
#include <algorithm>
#include <set>

using std::sort;
using std::set;

bool CComparator( const edge a, const edge b )
{
	return( a.weight > b.weight );
}

CApproximateSolution::CApproximateSolution( const vector<vector<float>> matrix_ )
{
	number_of_points = matrix_.size();
	matrix = matrix_;
	answer = 0;
	Solution();
}

void CApproximateSolution::transform_from_matrixGraph_to_setGraph()
{
	for( int i = 0; i < number_of_points; i++ ) {
		for( int j = i + 1; j < number_of_points; j++ ) {
			setGraph.push_back( edge( i, j, matrix[i][j] ) );
			setGraph.push_back( edge( j, i, matrix[i][j] ) );
		}
	}
	sort( setGraph.begin(), setGraph.end(), CComparator );
}

void CApproximateSolution::FindMST()
{
	transform_from_matrixGraph_to_setGraph();
	CSet sets( number_of_points, setGraph );
	MST = sets.MST;
}

void CApproximateSolution::Find_Eileur_path( int current, vector<vector<float>>& matrix_for_del )
{
	for( int i = 0; i < number_of_points; ++i ) {
		if( matrix_for_del[current][i] != 0 ) {
			matrix_for_del[current][i] = 0;
			Find_Eileur_path( i, matrix_for_del );
		}
	}
	eileurPath.push_back( current );
}

void CApproximateSolution::Find_result_path()
{
	set<int> visited;
	for( int i = 0; i < static_cast<int>(eileurPath.size()); ++i ) {
		if( visited.find( eileurPath[i] ) == visited.end() ) {
			resultPath.push_back( eileurPath[i] );
			visited.insert( eileurPath[i] );
		}
	}
	int num_of_vertices = static_cast<int>( resultPath.size() );
	for( int i = 0; i < num_of_vertices; ++i ) {
		answer += matrix[resultPath[i]][resultPath[( i + 1 ) % num_of_vertices]];
	}
}

void CApproximateSolution::Solution()
{
	FindMST();
	vector<vector<float>> matrix_for_del = MST;
	Find_Eileur_path( 0, matrix_for_del );
	Find_result_path();
}
