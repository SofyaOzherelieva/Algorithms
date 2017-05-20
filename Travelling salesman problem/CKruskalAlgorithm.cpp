#include "KruskalAlgorithm.h"


CSet::CSet( int number_of_vertices, const vector<edge>& setGraph )
{
	number_of_sets = number_of_vertices;
	sets.resize( number_of_vertices );
	// Create singleton  sets.
	for( int i = 0; i < number_of_vertices; i++ ) {
		sets[i].parent = &sets[i];
	}
	graph = setGraph;
	MST.resize( number_of_vertices );
	for( int i = 0; i < number_of_vertices; i++ ) {
		MST[i].resize( number_of_vertices );
		for( int j = 0; j < number_of_vertices; j++ ) {
			MST[i][j] = 0;
		}
	}
	kruskal_algorithm();
}

set_type* CSet::get_set_id( set_type* set )
{
	if( set == set->parent ) {
		return set;
	} else {
		set_type* parent = get_set_id( set->parent );
		set->parent = parent;
		return parent;
	}
}

void CSet::join_sets( set_type* set1, set_type* set2 )
{
	set1->parent = set2;
}

void CSet::kruskal_algorithm()
{
	int number_of_vertices = number_of_sets;
	for( ; number_of_sets > 1 && static_cast<int>( graph.size() ) > 0;) {
		edge current = graph.back();
		graph.pop_back();
		if( get_set_id( &sets[current.from] ) == get_set_id( &sets[current.to] ) ) {
			continue;
		}
		join_sets( get_set_id( &sets[current.from] ), get_set_id( &sets[current.to] ) );
		number_of_sets--;
		MST[current.from][current.to] = current.weight;
		MST[current.to][current.from] = current.weight;
	}
}