#pragma once

#include <string>

#include "../common.h"

#include "../Object.h"

class Str : public Object
{
private:
	std::string val;

	Object* add(Object*);
public:
	Str(std::string);
	Str(std::string*);

	std::string getVal();
};