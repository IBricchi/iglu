#include <iostream>
#include <string>

#include "scanner.h"

#include <vector>
#include "Chunk.h"
#include "VM.h"

using namespace std;

static void repl();
static void runFile(string path);

int main(int argc, char** argv)
{
	if (argc == 1) {
		//repl();
		Chunk c{};
		c.objects.push_back(new Object());
		c.objects[0]->val = (void*)new int(1);
		c.objects.push_back(new Object());
		c.objects[1]->val = (void*)new int(1);
		c.code.push_back((uint8_t)OpCode::OBJECT);
		c.code.push_back(0);
		c.code.push_back((uint8_t)OpCode::OBJECT);
		c.code.push_back(1);
		c.code.push_back((uint8_t)OpCode::ADD);
		c.code.push_back((uint8_t)OpCode::RETURN);
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
		vector<Token> tokens{};
		while (!scanner.isAtEnd()) {
			tokens.push_back(scanner.scanToken());
		}
	}
}

static void runFile(string path) {

}