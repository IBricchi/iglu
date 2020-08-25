#pragma once

#include <string>

#include "../common.h"

#include "../Object.h"

class Null : public Object {
private:
public:
	Null();

	std::string toString() override;
};