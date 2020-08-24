#include <iostream>
#include <string>

#include "VM.h"
#include "Scanner.h"
#include "Parser.h"
#include "Compiler.h"

using namespace std;

static void repl();
static void runFile(string path);

int main(int argc, char** argv)
{
	if (argc == 1) {
		repl();
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
	VM vm;
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
		Compiler compiler{};
		parser.parse();
		if (!parser.hadError) {
			compiler.compile(parser.getRPN());
			vm.interpret(compiler.getChunk());
		}
		int a = 1;
	}
}

static void runFile(string path) {

}