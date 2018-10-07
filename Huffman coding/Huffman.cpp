#include "Huffman.h"
#include <iostream>
#include <string>
#include <queue>
//#include "FileStreams.h"
#include <vector>
#include <algorithm>
#include <queue> 
#include <cmath>
using namespace std;
//#include "DebugFileWriter.h"
#include <functional>

#define LUNTIK false

std::vector<int> hornbook;

typedef unsigned char byte1;

struct SAlphabetSymbol{
	int frequency;
	unsigned char symbol;
	string code;
	SAlphabetSymbol* left; // Левый дочерний узел. NULL, если нет.
	SAlphabetSymbol* right; // Правый дочерний узел. NULL, если нет.
	SAlphabetSymbol() : frequency( 0 ), symbol( 0 ), left( NULL ), right( NULL ), code( "" ) {}
};

// Сортировка по убыванию.
void sort( vector<SAlphabetSymbol*>& hornbook, int size )
{
	for( int i = 0; i < size; i++ ){
		for( int j = i; j < size; j++ ){
			if( hornbook[i]->frequency <= hornbook[j]->frequency ){
				swap( hornbook[i], hornbook[j] );
			}
		}
	}
}
// Считает кол-во символов в алфавите. Использовать только на отсортированном!
int returnSize( vector<SAlphabetSymbol*>& hornbook )
{
	int size = 0;
	for( int i = 0; i < 256; i++ ){
		if( hornbook[i]->frequency != 0 ){
			size++;
		}
	}
	return size;
}

// Построение таблицы кодирования.
void InOrderAndBuildCodeTable( SAlphabetSymbol* node, string str, vector<string>& table )
{
	if( ( node->left == NULL ) && ( node->right == NULL ) ){
		node->code = str;
		// Записываем таблицу кодировки.
		table[node->symbol] = node->code;
		return;
	}
	string sL = str;
	string sR = str;
	sL += '0';
	sR += '1';
	InOrderAndBuildCodeTable( node->left, sL, table );
	InOrderAndBuildCodeTable( node->right, sR, table );
}
// Делает обход дерева слева-направо и печатает в консоль дерево, положенное на бок.
void printAsTree( SAlphabetSymbol* node, int currentLevel )
{
	if( node == NULL ){
		return;
	}
	printAsTree( node->left, currentLevel + 1 );
	cout << string( currentLevel, '\t' ) << "|" << (unsigned char) node->symbol << " " << node->frequency << " " << node->code << "|" << endl;
	printAsTree( node->right, currentLevel + 1 );
}

// Перевод из десятичной системы в двоичную восьмибитовую строку
string toBinaryCode( unsigned char code )
{
	string result = "";
	for( int i = 7; i >= 0; --i ){
		if( code >= pow( 2, i ) ){
			code -= pow( 2, i );
			result += '1';
		} else{
			result += '0';
		}
	}
	return result;
}
// Кодирование дерева.
// Обходим в глубину, узел - 0, лист - 1 и символ.
string BFSAndCodeTree( SAlphabetSymbol* node, string codeTree )
{
	queue<SAlphabetSymbol*> q;
	q.push( node );
	while( !q.empty() ){
		SAlphabetSymbol* node = q.front();
		q.pop();
		if( ( node->left == NULL ) && ( node->right == NULL ) ){
			codeTree += '1';
			string code = toBinaryCode( node->symbol );
			codeTree += code;
		} else{
			codeTree += '0';
			q.push( node->left );
			q.push( node->right );
		}
	}
	return codeTree;
}
// Вывод закодированной и сжатой последовательности.
string TreeToCode( vector<string>& table, SAlphabetSymbol* node )
{
	string codeTree = "";
	codeTree = BFSAndCodeTree( node, codeTree );
	if( LUNTIK ){
		cout << "CodeTree" << '\n';
		cout << codeTree;
	}
	return codeTree;
}
// Перевод из двоичного числа в десятичное.
byte bitToDec( string& oneByte )
{
	byte rez = 0;
	for( int i = 0; i < 8; i++ ){
		if( oneByte[i] == '1' ){
			rez += pow( 2, ( 8 - 1 - i ) );
		}
	}
	return rez;
}

// Вывод по байтам.
void byteWrite( IOutputStream& compressed, string outPut )
{
	int currPos = 7;
	unsigned char buffer = 0;
	int strSize = (int) outPut.size();
	int ind = 0;
	while( true ){
		for( int i = 7; i >= 0; i-- ){
			if( ind >= strSize ){
				if( i == 7 ){
					compressed.Write( buffer );
					if( LUNTIK ) cout << buffer << " ";
					return;
				} else{
					while( currPos >= 0 ){
						buffer |= ( 0 << currPos );
						currPos--;
					}
					compressed.Write( buffer );
					if( LUNTIK ) cout << buffer << " ";
					return;
				}
			}
			int bit = outPut[ind] - '0';
			buffer |= ( bit << currPos );
			++ind;
			if( currPos == 0 ){
				compressed.Write( buffer );
				buffer = 0;
				currPos = 7;
			} else{
				currPos--;
			}
		}
	}
}
// Разрушает поддерево, корнем которого является node.
void deleteSubTree( SAlphabetSymbol* node )
{
	if( node->code == "" ){
		// Дошли до конца дерева.
		return;
	}
	if(node->left != NULL )	deleteSubTree( node->left );
	node->left = 0;
	if( node->right != NULL )deleteSubTree( node->right );
	node->right = 0;
	delete node;
}
//bool comp( SAlphabetSymbol *a, SAlphabetSymbol *b )
//{
//	return a->frequency > b->frequency;
//}
struct Compare
{
	bool operator() ( SAlphabetSymbol *a, SAlphabetSymbol *b )
	{
		return a->frequency > b->frequency;
	}
};
// Метод архивирует данные из потока original
void Encode( IInputStream& original, IOutputStream& compressed )
{
	vector<string> table( 256 );
	vector<SAlphabetSymbol*> hornbook( 256, new SAlphabetSymbol() );
	vector<byte1> originalFile;
	SAlphabetSymbol abc;
	for( int i = 0; i < 256; ++i ){
		hornbook[i] = new SAlphabetSymbol();
	}
	byte1 symbol = 0;
	byte temp = symbol;
	while( original.Read( temp ) ){
		symbol = temp;
		originalFile.push_back( symbol );
		hornbook[symbol]->symbol = symbol;
		hornbook[symbol]->frequency++;
		//compressed.Write( symbol );
	}
	int size = 256;
	sort( hornbook, size );
	size = returnSize( hornbook );
	if( LUNTIK ){
		cout << "Alphabet and frequency" << '\n';
		for( int i = 0; i < size; i++ ){
			cout << (unsigned char) hornbook[i]->symbol << " " << hornbook[i]->frequency << '\n';
		}
		cout << size;
		cout << '\n';
	}

	priority_queue<SAlphabetSymbol*, vector<SAlphabetSymbol*>, Compare> QueueOfTrees;
	for( int i = 0; i < size; i++ ){
		QueueOfTrees.push( hornbook[i] );
	}
	// Построение очереди с приоритетами.
	while( QueueOfTrees.size() > 1 ){
		SAlphabetSymbol* littleTree = new SAlphabetSymbol();
		littleTree->left = QueueOfTrees.top();
		QueueOfTrees.pop();
		littleTree->right = QueueOfTrees.top();
		QueueOfTrees.pop();
		littleTree->frequency = littleTree->left->frequency + littleTree->right->frequency;
		littleTree->symbol = 0;
		QueueOfTrees.push( littleTree );
	}
	SAlphabetSymbol* tree = QueueOfTrees.top();
	InOrderAndBuildCodeTable( tree, "", table );
	if( LUNTIK ){
		cout << "Code tree" << '\n';
		printAsTree( tree, 0 );
		cout << "Code table" << '\n';
		for( int i = 0; i <= 255; i++ ){
			if( table[i] != "" ){
				cout << (unsigned char) i << " " << table[i] << '\n';
			}
		}
	}
	string encodeFile = "";
	for( int i = 0; i < originalFile.size(); i++ ){
		encodeFile += table[originalFile[i]];
	}
	if( LUNTIK ){
		cout << "EncodeFile" << '\n';
		cout << encodeFile << '\n';
	}
	string codeTree = TreeToCode( table, tree );
	string outPut = "";
	outPut += codeTree;
	outPut += encodeFile;
	if( LUNTIK )cout << "\n\n\n";
	byte exCount = 8 - outPut.size() % 8;
	compressed.Write( exCount );
	deleteSubTree( tree );
	for( int i = 0; i < 256; i++ ){
		deleteSubTree( hornbook[i] );
	}
	byteWrite( compressed, outPut );
}
void readTree( SAlphabetSymbol* node, string& data, int& i, queue<SAlphabetSymbol*>& q )
{
	if( data[i] == '0' ){
		SAlphabetSymbol* leftTree = new SAlphabetSymbol();
		SAlphabetSymbol* rightTree = new SAlphabetSymbol();
		node->left = leftTree;
		q.push( leftTree );
		node->right = rightTree;
		q.push( rightTree );
	}
	if( data[i] == '1' ){
		i++;
		string newSymbol = "";
		for( int j = 0; j < 8; j++ ){
			newSymbol += data[i];
			i++;
		}
		i--;
		node->symbol = (char) bitToDec( newSymbol );
	}
	if( q.empty() ) return;
	node = q.front();
	q.pop();
	i++;
	readTree( node, data, i, q );
}
void decodeFile( SAlphabetSymbol* root, int& i, string& data, IOutputStream& original )
{
	SAlphabetSymbol* node = root;
	while( node->left != NULL && node->right != NULL ){
		if( data[i] == '0' ){
			node = node->left;
		} else{
			node = node->right;
		}
		i++;
	}
	original.Write( (char) node->symbol );
}
// Метод восстанавливает оригинальные данные
void Decode( IInputStream& compressed, IOutputStream& original )
{
	vector<string> decodeTable( 256 );
	string data = "";
	byte symbol = 0;
	byte exCount = 0;
	compressed.Read( exCount );
	while( compressed.Read( symbol ) ){
		data += toBinaryCode( symbol );
		//original.Write( symbol );
	}
	if( LUNTIK ){
		cout << '\n';
		cout << "deCode data";
		cout << data << '\n';
	}
	data.erase( data.size() - exCount, exCount ); //Удаляем последние ненужные нули
	if( LUNTIK ){
		cout << '\n';
		cout << "deCode data";
		cout << data << '\n';
	}
	SAlphabetSymbol* root = new SAlphabetSymbol();
	queue<SAlphabetSymbol*> q;
	bool temp = true;
	int i = 0;
	readTree( root, data, i, q );
	if( LUNTIK ){
		cout << "deCode tree" << '\n';
		printAsTree( root, 0 );
	}
	InOrderAndBuildCodeTable( root, "", decodeTable );
	if( LUNTIK ){
		cout << "deCode table" << '\n';
		for( int i = 0; i <= 255; i++ ){
			if( decodeTable[i] != "" ){
				cout << (unsigned char) i << " " << decodeTable[i] << '\n';
			}
		}
	}
	i++;
	while( i < data.size() ){
		decodeFile( root, i, data, original );
	}
	deleteSubTree( root );
}