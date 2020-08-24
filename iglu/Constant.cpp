#include "Constant.h"

using namespace std;

Constant::Constant() : type(ConstType::NILL), val(as{ false }) {}
Constant::Constant(bool val) : type(ConstType::BOOL), val(val) {}
Constant::Constant(float val) : type(ConstType::NUMBER), val(val) {}
Constant::Constant(std::string& val) : type(ConstType::STRING), val(val) {}
Constant::~Constant() {
	if (type == ConstType::STRING) {
		delete val.String;
	}
}