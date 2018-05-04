#include "Query.h"
#include "TextQuery.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

using std::set;
using std::string;
using std::map;
using std::vector;
using std::cerr;
using std::cout;
using std::cin;
using std::ifstream;
using std::endl;

int main(int argc, char **argv)
{
	TextQuery file = get_file(argc, argv);

	do {
		string sought;
		if (!get_word(sought)) break;

		Query name(sought);
		const QueryResult results = name.eval(file);
		cout << "\nExcuting Query for: " << name << endl;

		print(cout, results) << endl;  //print()函数在TextQuery.h中声明
	} while (true);
	return 0;
}



