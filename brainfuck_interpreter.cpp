#include <iostream>
#include <vector>
#include <stack>
#include <deque>
#include <fstream>
#include <cstdlib>

using std::vector;
using std::stack;
using std::cout;
using std::cin;
using std::ifstream;

/****************************************************************/

/* function prototypes */
void read(vector<char> &i_vc, ifstream &is);
void interp(vector<unsigned char> &cells, vector<char> &ivc);
vector<char>::iterator skip_block(vector<char>::iterator i_it);

/****************************************************************/

/* reads brainfuck code into memory 
 */
void read(vector<char> &i_vc, ifstream &is) {
	char c;
	is.get(c);

	while (!is.eof()) {
		i_vc.push_back(c);
		is.get(c);
	}
}

/****************************************************************/

/* interprets the brainfuck code, and performs the actions
 */
void interp(vector<unsigned char> &cells, vector<char> &i_vc) {
	stack<vector<char>::iterator> block_front;
	auto c_it = cells.begin();
	
	// scan through the code character by character
	for (auto i_it = i_vc.begin(); i_it != i_vc.end(); ++i_it) {
		if (*i_it == '>') {
			++c_it;
		} else if (*i_it == '<') {
			--c_it;
		} else if (*i_it == '+') {
			++(*c_it);
		} else if (*i_it == '-') {
			--(*c_it);
		} else if (*i_it == '.') {
			cout.put(*c_it);
		} else if (*i_it == ',') {
			*c_it = cin.get();
		} else if (*i_it == '[') {
			if (*c_it) {
				block_front.push(i_it);
			} else {
				i_it = skip_block(i_it);
			}
		} else if (*i_it == ']') {
			if (*c_it) {
				i_it = block_front.top();
			} else {
				block_front.pop();
			}
		}
	}
}

/****************************************************************/

/* skips to the end of a block of brainfuck code, indicated via [ and ], and
 * returns an iterator to it
 */
vector<char>::iterator skip_block(vector<char>::iterator i_it) {
	int count = 0;

	for (++i_it; ; ++i_it) {
		if (*i_it == ']' && count == 0) {
			return i_it;
		} else if (*i_it == ']') {
			--count;
		} else if (*i_it == '[') {
			++count;
		}
	}
}

/****************************************************************/

int main(int argc, char *argv[]) {
	const int Cells = 30000;
	vector<unsigned char> cells(Cells, 0);
	vector<char> i_vc;
	
	if (argc < 2) {
		// no brainfuck file provided
		cout << "Usage: brainfuck_interpreter "
			"<input code file name>\n";
			return EXIT_FAILURE;
	}
	ifstream is(argv[1]);
	read(i_vc, is);
	interp(cells, i_vc);
	return 0;
}

