#include <iostream>
#include <string>


#include <vector>
#include "Scanner.h"
#include "Parser.h"
#include "Chunk.h"
#include "VM.h"

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