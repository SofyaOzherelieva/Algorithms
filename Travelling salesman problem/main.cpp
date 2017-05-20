#define _USE_MATH_DEFINES

#include <cmath>
#include "Graph.h"
#include "ExactSolution.h"
#include "ApproximateSolution.h"

#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>

using std::pair;
using std::make_pair;
using std::cin;
using std::cout;
using std::ofstream;

ofstream fout( "result.txt" );

float find_mean( const vector<pair<float, float>> results )
{
	float sum = 0;
	for( int i = 0; i < static_cast<int>( results.size() ); i++ ) {
		sum += results[i].second;
	}
	float mean = sum / results.size();
	return mean;
}

// Calculation of standard deviation
float find_standard_deviation( const vector<pair<float, float>> results )
{
	float sum = 0;
	for( int i = 0; i < static_cast<int>( results.size() ); i++ ) {
		sum += pow(results[i].first - results[i].second, 2);
	}
	float deviation = sqrt(sum) / results.size();
	return deviation;
}


void show( vector<vector<float>>& matrix )
{
	int number_of_vertices = matrix.size();
	for( int i = 0; i < number_of_vertices; i++ ) {
		for( int j = 0; j < number_of_vertices; j++ ) {
			fout << matrix[i][j] << "     ";
		}
		fout << '\n';
	}
	fout << '\n';
	fout << '\n';
}

int main()
{
	srand( static_cast<unsigned int>(time( 0 )));

	int number_of_experiments, number_of_points;
	cout << "Insert number of experiments, please: ";
	cin >> number_of_experiments;
	for( int number_of_points = 2; number_of_points <= 10; ++number_of_points ) {
		fout << "Number of points is \n";
		fout << number_of_points << '\n' << '\n';
		vector<pair<float, float>> results;
		for( int i = 0; i < number_of_experiments; i++ ) {
			// Create a random matrix.
			CGraph graph( number_of_points );
			vector<vector<float>> random_matrix = graph.Generate_matrix();
			
			show( random_matrix );

			CExactSolution exact_solution( random_matrix );
			CApproximateSolution approximate_solution( random_matrix );
			float exact = exact_solution.answer;
			float approximate = approximate_solution.answer;
			fout << "exact: " << exact << " approximate: " << approximate << '\n';
			results.push_back( make_pair( exact, approximate ) );
		}
		fout << "Mean: \n";
		fout << find_mean( results ) << '\n';
		fout << "Standard deviation: \n";
		fout << find_standard_deviation( results ) << '\n';
		results.clear();
	}
	cout << "All done \n";
	fout.close();

	return 0;
}
