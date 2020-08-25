#pragma once

#include <string>

#include "../common.h"

#include "../Object.h"

class Bool : public Object
{
private:
	bool val;
public:
	Bool(bool);

	std::string toString() override;
	bool getVal();
};