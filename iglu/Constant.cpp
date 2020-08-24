#include "Constant.h"

using namespace std;

Constant::Constant() : type(ConstType::NILL), as(false) {}
Constant::Constant(bool val) : type(ConstType::BOOL), as(val) {}
Constant::Constant(float val) : type(ConstType::NUMBER), as(val) {}
Constant::Constant(std::string& val) : type(ConstType::STRING), as(val) {}