#include <iostream>
#include <string>


#include <vector>
#include "Scanner.h"
#include "Parser.h"
#include "Chunk.h"
#include "Object.h"
#include "objects/Number.h"
#include "VM.h"

using namespace std;

static void repl();
static void runFile(string path);

int main(int argc, char** argv)
{
	if (argc == 1) {
		Chunk c{};
		c.code.push_back((uint8_t) OpCode::OBJECT);
		c.code.push_back(0);
		c.code.push_back((uint8_t) OpCode::OBJECT);
		c.code.push_back(1);
		c.code.push_back((uint8_t) OpCode::PLUS);
		c.code.push_back((uint8_t) OpCode::RETURN);

		c.objects.push_back(new Number(1));
		c.objects.push_back(new Number(2));

		VM vm{};
		vm.interpret(&c);
	}
	else if (argc == 2) {
		runFile(string(argv[1]));
	}
	else {
		cerr << "Usage: iglu [path]?" << endl;
	}

	return 0;
}

static void repl() {
	string line;
	while (true) {
		cout << "~> ";
		getline(cin, line);
		if (cin.fail()) {
			cout << endl;
			break;
		}
		Scanner scanner(&line[0]);
		Parser parser(&scanner);
		parser.parse();
	}
}

static void runFile(string path) {

}