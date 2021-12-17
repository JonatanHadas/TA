#ifndef _SERIALIZATION_H
#define _SERIALIZATION_H

#include <iostream>
#include <ostream>
#include <istream>

using namespace std;

template<typename T>
void write_raw(ostream& output, const T& value){
	output.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

template<typename T>
T read_raw(istream& input){
	T value;
	input.read(reinterpret_cast<char*>(&value), sizeof(T));
	return value;
}

#endif