#pragma once

#include <string>

#include "../common.h"

#include "../Object.h"

class Bool : public Object
{
private:
	bool val;

	Object* _not();
	Object* equal(Object*);
	Object* notEqual(Object*);

public:
	Bool(bool);

	std::string toString() override;
	bool getVal();
};