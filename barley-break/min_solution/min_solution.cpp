#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cassert> 
#define FIELD 16
#define S_FIELD 4
#define MAX_STEPS 100
//#define TARGET 2271560481
#define MAX 32767
#define TARGET 1147797409030816545


using std::vector;
using std::string;
using std::swap;
using std::endl;

#include <sstream>
#include <string>

class CPosition {
public:
	explicit CPosition( const string& source );

	int ManhattanDistance() const;

	vector<CPosition> add_next_states() const;

	bool is_there_a_solution() const;
	unsigned long long data; // 16 ячеек по 4 бита каждая.
	
	char a_way;
private:
	// Передвижение пустышки.
	CPosition Up() const;
	CPosition Down() const;
	CPosition Right() const;
	CPosition Left() const;

	int nullPlace;
	
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
	a_way( ' ' )
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

vector<CPosition> CPosition::add_next_states() const
{
	vector<CPosition> next_states;
	next_states.reserve( 3 );
	// Up:
	if( nullPlace >= S_FIELD && a_way != 'U' ) {
		CPosition new_pos = Up();
		next_states.push_back( new_pos );
	}
	// Down:
	if( nullPlace <= ( FIELD - S_FIELD - 1 ) && a_way != 'D' ) {
		CPosition new_pos = Down();
		next_states.push_back( new_pos );
	}
	// Left:
	if( ( ( nullPlace + 1 ) % (int) S_FIELD != 1 ) && a_way != 'L' ) {
		CPosition new_pos = Left();
		next_states.push_back( new_pos );
	}
	// Right:
	if( ( ( nullPlace + 1 ) % S_FIELD != 0 ) && a_way != 'R' ) {
		CPosition new_pos = Right();
		next_states.push_back( new_pos );
	}
	return next_states;
}

// Эвристика.
int CPosition::ManhattanDistance() const
{
	int number_of_moves = 0;
	for( int i = 0; i < FIELD; i++ ) {
		int current_position = getAt( i );
		if( current_position != 0 ) {
			number_of_moves += abs( ( current_position - 1 ) % S_FIELD - ( i ) % S_FIELD ) + abs( ( current_position - 1 ) / S_FIELD - ( i ) / S_FIELD );
		}
	}
	return number_of_moves;
}

// Сортируем по убыванию ManhattanDistance.
void sort( vector<CPosition>& states )
{
	for( int i = 0; i < static_cast<int>( states.size() ); i++ ) {
		for( int j = i + 1; j < static_cast<int>( states.size() ); j++ ) {
			if( states[i].ManhattanDistance() < states[j].ManhattanDistance() ) {
				swap( states[i], states[j] );
			}
		}
	}
}

class CIDA {
public:
	explicit CIDA( const CPosition& state );

	bool IDAstar();
	vector<char> return_result() const;
private:
	bool search( const CPosition& state, int number_of_steps );
	int min;
	int deepness = 0;
	vector<char> answer;
	CPosition puzzle;
};

CIDA::CIDA( const CPosition& state ) :
	min( 0 ),
	deepness( state.ManhattanDistance() ),
	puzzle (state)
{}


// Вывод ответа
vector<char> CIDA::return_result() const
{
	return answer;
}

bool CIDA::search( const CPosition& state, int number_of_steps )
{
	int h = state.ManhattanDistance();
	if( h == 0 ) {
		return true;
	}
	int f = state.ManhattanDistance() + number_of_steps;
	if( f > deepness ) {
		if( min > f ) {
			min = f;
		}
		return false;
	}
	vector<CPosition> next_states = state.add_next_states();
	sort( next_states );
	for( CPosition next : next_states ) {
		bool result = search( next, number_of_steps + 1 );
		if( result == true ) {
			answer.push_back( next.a_way ); 
			return true;
		}
	}
	return false;
}

bool CIDA::IDAstar()
{
	bool result = false;
	while( deepness <= 1000 ) {
		min = MAX;
		result = search( puzzle, 0 );
		deepness = min;
		if( result ) {
			break;
		}
	}
	return result;
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
		std::cout << 0 << endl << "" << endl;
		return 0;
	}
	if( pos.is_there_a_solution() ) {
		CIDA IDA( pos );
		IDA.IDAstar();
		vector<char> answer = IDA.return_result();
		std::cout << static_cast<int>( answer.size() ) << endl;
		for( int i = static_cast<int>( answer.size() ) - 1; i >= 0; --i ) {
			std::cout << answer[i];
		}
	} else {
		std::cout << -1 << endl;
	}
	return 0;
}