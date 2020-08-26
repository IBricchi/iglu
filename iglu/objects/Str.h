#pragma once

#include <string>

#include "../common.h"

#include "../Object.h"

class Str : public Object
{
private:
	std::string val;

	Object* concat(Object*);
	Object* compare(Object*);
	Object* notCompare(Object*);
public:
	Str(std::string);
	Str(std::string*);

	std::string toString() override;
	std::string getVal();
};