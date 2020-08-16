#include <iostream>
#include <string>

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

}

static void runFile(string path) {

}