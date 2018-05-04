#include "Query.h"
#include "TextQuery.h"
#include <string>
#include <set>
#include <iostream>

using std::set;
using std::string;
using std::cin; using std::cout; using std::cerr;
using std::endl;


int main(int argc, char **argv)
{
	
	TextQuery file = get_file(argc, argv);

	while (true) {
		string sought1, sought2, sought3;
		if (!get_words(sought1, sought2)) break;
		cout << "\nenter third word: ";
		cin >> sought3;
		Query q = Query(sought1) & Query(sought2)
			| Query(sought3);
		cout << "\nExecuting Query for: " << q << endl;
		const QueryResult results = q.eval(file);
		print(cout, results);
	}
	return 0;
}


