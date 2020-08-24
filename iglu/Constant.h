#pragma once

#include <string>

enum struct ConstType{
	NILL,
	BOOL,
	NUMBER,
	STRING
};

union as {
	bool Bool;
	float Number;
	std::string* String;

	as(bool val) {Bool = val;}
	as(float val) {Number = val;}
	as(std::string& val) {String = new std::string(val);}
};

struct Constant
{
	const ConstType type;
	const as val;

	Constant();
	Constant(bool);
	Constant(float);
	Constant(std::string&);
	~Constant();
};

