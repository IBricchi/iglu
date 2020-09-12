#pragma once

#include <string>

#include "../common.h"

#include "../Object.h"

class Error : public Object {
private:
	std::string message;

	Object* toString();
public:
	Error(std::string);

	std::string getMessage();
};