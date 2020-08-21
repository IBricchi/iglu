#pragma once

#include <string>

#include "../common.h"

#include "../Object.h"

class Error : public Object {
private:
	std::string message;
public:
	Error(std::string);

	std::string getMessage();
};