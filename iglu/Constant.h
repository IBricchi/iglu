#pragma once

#include <string>

enum struct ConstType{
	NILL,
	BOOL,
	NUMBER,
	STRING
};

union As {
	bool Bool;
	float Number;
	std::string* String;

	As(bool val) {Bool = val;}
	As(float val) {Number = val;}
	As(std::string& val) {String = new std::string(val);}
};

struct Constant
{
	const ConstType type;
	const As as;

	Constant();
	Constant(bool);
	Constant(float);
	Constant(std::string&);
};

