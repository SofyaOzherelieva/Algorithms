#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <string>
#include <cassert> 
#include <unordered_set>
#include <set>
#include <unordered_map>
#define FIELD 16
#define S_FIELD 4
//#define TARGET 2271560481
#define MAX 32767
#define MAX_SIZE 649
#define TARGET 1147797409030816545


using std::vector;
using std::queue;
using std::pair;
using std::string;
using std::swap;
using std::unordered_set;
using std::set;
using std::priority_queue;
using std::unordered_map;

#include <assert.h>
#include <sstream>
#include <string>

class CPosition {
public:
	explicit CPosition( const std::string& source );

	// Передвижение пустышки.
	CPosition Up() const;
	CPosition Down() const;
	CPosition Right() const;
	CPosition Left() const;

	bool is_there_a_solution() const;
	unsigned long long data; // 16 ячеек по 4 бита каждая.
							 //string way; // Путь до состояния.
	char a_way;
	int nullPlace;
	int number_of_steps;

	void setAt( int place, unsigned char value );
	unsigned char getAt( int place ) const;
};

const unsigned long long Masks[] = {
	0x000000000000000F,
	0x00000000000000F0,
	0x0000000000000F00,
	0x000000000000F000,
	0x00000000000F0000,
	0x0000000000F00000,
	0x000000000F000000,
	0x00000000F0000000,
	0x0000000F00000000,
	0x000000F000000000,
	0x00000F0000000000,
	0x0000F00000000000,
	0x000F000000000000,
	0x00F0000000000000,
	0x0F00000000000000,
	0xF000000000000000,
};

const unsigned long long AntiMasks[] = {
	0xFFFFFFFFFFFFFFF0,
	0xFFFFFFFFFFFFFF0F,
	0xFFFFFFFFFFFFF0FF,
	0xFFFFFFFFFFFF0FFF,
	0xFFFFFFFFFFF0FFFF,
	0xFFFFFFFFFF0FFFFF,
	0xFFFFFFFFF0FFFFFF,
	0xFFFFFFFF0FFFFFFF,
	0xFFFFFFF0FFFFFFFF,
	0xFFFFFF0FFFFFFFFF,
	0xFFFFF0FFFFFFFFFF,
	0xFFFF0FFFFFFFFFFF,
	0xFFF0FFFFFFFFFFFF,
	0xFF0FFFFFFFFFFFFF,
	0xF0FFFFFFFFFFFFFF,
	0x0FFFFFFFFFFFFFFF
};

CPosition::CPosition( const std::string& source ) :
	data( 0 ),
	nullPlace( 0 ),
	a_way( ' ' ),
	number_of_steps( 0 )
{
	std::istringstream stringStream( source );
	for( char i = 0; i < FIELD; ++i ) {
		unsigned short value = 0;
		stringStream >> value;
		setAt( i, static_cast<unsigned char>( value ) );
		if( value == 0 ) {
			nullPlace = i;
		}
	}
}

// Установка значения в некоторую позицию.
void CPosition::setAt( int place, unsigned char value )
{
	data = ( data & AntiMasks[place] ) | ( static_cast<long long>( value ) << ( place << 2 ) );
}

// Получение того, что лежит в некоторой позиции.
unsigned char CPosition::getAt( int place ) const
{
	return static_cast<unsigned char>( ( data & Masks[place] ) >> ( place << 2 ) );
}

CPosition CPosition::Up() const
{
	CPosition result( *this );

	// Ставим пустышку выше.
	result.setAt( nullPlace - S_FIELD, 0 );
	// Ставим то, что было выше, на то место, где была пустышка.
	result.setAt( nullPlace, getAt( nullPlace - S_FIELD ) );
	result.nullPlace -= S_FIELD;
	result.number_of_steps += 1;
	result.a_way = 'D';
	return result;
}

CPosition CPosition::Down() const
{
	CPosition result( *this );

	// Ставим пустышку ниже.
	result.setAt( nullPlace + S_FIELD, 0 );
	// Ставим то, что было ниже, на то место, где была пустышка.
	result.setAt( nullPlace, getAt( nullPlace + S_FIELD ) );
	result.nullPlace += S_FIELD;
	result.number_of_steps += 1;
	result.a_way = 'U';
	return result;
}

CPosition CPosition::Right() const
{
	CPosition result( *this );

	// Ставим пустышку правее.
	result.setAt( nullPlace + 1, 0 );
	// Ставим то, что было правее, на то место, где была пустышка.
	result.setAt( nullPlace, getAt( nullPlace + 1 ) );
	result.nullPlace += 1;
	result.number_of_steps += 1;
	result.a_way = 'L';
	return result;
}

CPosition CPosition::Left() const
{
	CPosition result( *this );

	// Ставим пустышку левее.
	result.setAt( nullPlace - 1, 0 );
	// Ставим то, что было левее, на то место, где была пустышка.
	result.setAt( nullPlace, getAt( nullPlace - 1 ) );
	result.nullPlace -= 1;
	result.number_of_steps += 1;
	result.a_way = 'R';
	return result;
}

// Проверяем собирется ли головоломка.
bool CPosition::is_there_a_solution() const
{
	int counter = 0;
	for( int i = 0; i < FIELD; i++ ) {
		for( int j = i + 1; j < FIELD; j++ ) {
			if( getAt( j ) < getAt( i ) && getAt( j ) != 0 ) {
				counter++;
			}
		}
	}
	if( FIELD != 9 ) {
		counter += ( nullPlace / S_FIELD + 1 );
	}
	if( counter % 2 == 0 ) {
		return true;
	} else {
		return false;
	}
}

vector<CPosition> add_next_states( CPosition current )
{
	vector<CPosition> next_states;
	next_states.reserve( 4 );
	unsigned long long state = current.data;
	int empty = current.nullPlace;
	// Up:
	if( empty >= S_FIELD ) {
		CPosition new_pos = current.Up();
		next_states.push_back( new_pos );
	}
	// Down:
	if( empty <= ( FIELD - S_FIELD - 1 ) ) {
		CPosition new_pos = current.Down();
		next_states.push_back( new_pos );
	}
	// Left:
	if( empty >= 1 && ( ( empty + 1 ) % (int) S_FIELD != 1 ) ) {
		CPosition new_pos = current.Left();
		next_states.push_back( new_pos );
	}
	// Right:
	if( empty <= FIELD - 2 && ( ( empty + 1 ) % S_FIELD != 0 ) ) {
		CPosition new_pos = current.Right();
		next_states.push_back( new_pos );
	}
	return next_states;
}

// Эвристика.
int hash( CPosition state )
{
	int number_of_moves = 0;
	for( int i = 0; i < FIELD; i++ ) {
		int current_position = state.getAt( i );
		if( current_position == 0 ) {
			//number_of_moves += abs( ( i ) % S_FIELD - ( FIELD - 1 ) % S_FIELD ) + abs( ( i ) / S_FIELD - ( FIELD - 1 ) / S_FIELD );
		} else {
			number_of_moves += abs( ( current_position - 1 ) % S_FIELD - ( i ) % S_FIELD ) + abs( ( current_position - 1 ) / S_FIELD - ( i ) / S_FIELD );
		}
	}
	return number_of_moves;
}

int heur( CPosition current, CPosition next )
{
	return abs( hash( current ) - hash( next ) );
}


string restore_way( CPosition current_state, unordered_map<unsigned long long, char>& isVisited, unsigned long long first_pos )
{
	vector<char> mass_result;
	string result = "";
	CPosition pos = current_state;
	while( pos.data != first_pos ) {
		if( isVisited[pos.data] == 'D' ) {
			pos = pos.Down();
			mass_result.push_back( 'D' );
		} else if( isVisited[pos.data] == 'U' ) {
			pos = pos.Up();
			mass_result.push_back( 'U' );
		} else if( isVisited[pos.data] == 'L' ) {
			pos = pos.Left();
			mass_result.push_back( 'L' );
		} else {
			pos = pos.Right();
			mass_result.push_back( 'R' );
		}
	}
	for( int i = mass_result.size() - 1; i >= 0; i-- ) {
		result += mass_result[i];
	}
	return result;
}

class CComparator {
public:
	bool operator()( const pair<int, CPosition> a, const pair<int, CPosition> b )
	{
		return( a.first == b.first ? a.second.data > b.second.data : a.first < b.first );
	}
};

string Astar( CPosition state )
{
	unordered_map<unsigned long long, char> parent; // Множество рассмотренных вершин
	unordered_map<unsigned long long, int> distance;

	unordered_set<unsigned long long> isVisited;

	unsigned long long first_pos = state.data;
	set <pair<int, CPosition>, CComparator> q; // Множество вершин, которые надо рассмотреть
	q.insert( { 0, state } );
	while( !q.empty() ) {
		CPosition current_state = q.begin()->second;
		if( current_state.data == TARGET ) {
			return restore_way( current_state, parent, first_pos );
		}
		q.erase( q.begin() );
		if( isVisited.find( current_state.data ) != isVisited.end() ) {
			continue;
		}
		isVisited.insert( current_state.data );
		parent[current_state.data] = current_state.a_way;
		parent[current_state.data] = current_state.a_way;
		vector<CPosition> next_states = add_next_states( current_state );
		for( int i = 0; i < next_states.size(); i++ ) {
			CPosition next = next_states[i];
			if( isVisited.find( next.data ) != isVisited.end() ) {
				continue;
			}
			if( ( current_state.number_of_steps + 1 + hash( next ) < distance[next.data] ) || distance.find( next.data ) != distance.end() ) {

				distance[next.data] = next.number_of_steps + hash( next );
				next.number_of_steps = current_state.number_of_steps + 1;
				q.insert( { distance[next.data], next } );
				parent[next.data] = next.a_way;
			}
			// Если кол-во элементов в set привышает какой-то порог - удаляем
			if( q.size() > MAX_SIZE ) {
				while( q.size() > MAX_SIZE * 4/5 ) {
					q.erase( --q.end() );
				}
			}
		}
	}
}

int main()
{
	string number = "";
	string symbol;
	for( int i = 0; i < FIELD; i++ ) {
		std::cin >> symbol;
		if( symbol != "" && symbol != "\n" ) {
			number += symbol + " ";
		}
	}
	CPosition pos( number );
	if( pos.data == TARGET ) {
		std::cout << 0 << "\n" << "" << "\n";
		return 0;
	}
	if( pos.is_there_a_solution() ) {
		string result = Astar( pos );
		std::cout << result.length() << "\n" << result << "\n";
	} else {
		std::cout << -1 << "\n";
	}
	return 0;
}

/*int main()
{
std::ifstream fin( "puzzle.in" );
std::ofstream fout( "puzzle.out" );
string number = "";
string symbol;
for( int i = 0; i < FIELD; i++ ) {
fin >> symbol;
if( symbol != "" && symbol != "\n" ) {
number += symbol + " ";
}
}
CPosition pos( number );
if( pos.data == TARGET ) {
string result = "";
fout << result.length() << "\n" << result << "\n";
return 0;
}
if( pos.is_there_a_solution() ) {
string result = Astar( pos );
fout << result.length() << "\n" << result << "\n";
} else {
fout << -1;
}
return 0;
}*/