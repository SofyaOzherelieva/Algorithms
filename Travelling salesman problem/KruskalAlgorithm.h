#pragma once
#include "ApproximateSolution.h"


struct set_type {
	set_type* parent; // link on parent
};

class CSet {
public:
	CSet( int number_of_vertices,const vector<edge>& setGraph );
	void kruskal_algorithm();
	vector<vector<float>> MST;
private:
	set_type* get_set_id( set_type* set );
	void join_sets( set_type* set1, set_type* set2 );

	int number_of_sets;
	vector<edge> graph;
	vector<set_type> sets;
};
