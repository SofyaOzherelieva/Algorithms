#pragma once

#define _USE_MATH_DEFINES

#include "TestGenerator.h"
#include <cmath>

using std::make_pair;


CGenerator::CGenerator( int number_of_points_ )
{
	matrix.resize( number_of_points_ );
	for( int i = 0; i < number_of_points_; i++ ) {
		matrix[i].resize( number_of_points_ );
		for( int j = 0; j < number_of_points_; j++ ) {
			matrix[i][j] = 0;
		}
	}
	number_of_points = number_of_points_;
	rand_points.reserve( number_of_points_ );
	BoxMuller_transform();
}

float CGenerator::Float_rand()
{
	float result = static_cast <float> ( rand() ) / static_cast <float> ( RAND_MAX );
	while( result == 0 ) {
		result = static_cast <float> ( rand() ) / static_cast <float> ( RAND_MAX );
	}
	return result;
}

void CGenerator::BoxMuller_transform()
{

	for( int i = 0; i < number_of_points; ++i ) {
		float r = Float_rand();
		float fi = Float_rand();
		float z0 = cos( 2 * M_PI * fi ) * sqrt( -2 * log( r ) );
		float z1 = sin( 2 * M_PI * fi ) * sqrt( -2 * log( r ) );
		rand_points.push_back( make_pair( z0, z1 ) );
	}
}

vector<pair<float, float>> CGenerator::Return_rand_points()
{
	return rand_points;
}