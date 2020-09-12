#pragma once

#include <string>

#include "../common.h"

#include "../Object.h"

class Null : public Object {
private:
	Object* toString();
public:
	Null();
};