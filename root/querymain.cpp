#include <iostream> //iostream定义了读写流的基本类型
#include <fstream> //fstream定义了读写命名文件的类型

#include <string>
using std::string;

#include <fstream>
using std::ifstream;

#include <iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include <cstdlib> //for EXIT_FAILURE

#include "TextQuery.h"
#include "make_plural.h"


//tip:在真正实现成员之前先编写程序使用这个类 这样可以看到类是否具有所需要的操作
void runQueries(ifstream &infile)
{
	//infile是一个ifstream，指向要处理的文件
	TextQuery tq(infile); //保存文件并建立查询map [用到构造函数 TextQuery::TextQuery(ifstream &is) :file(new vector<string>)]
	//与用户交互：提示用户输入要查询的单词，完成查询并打印结果
	while (true) {
		cout << "enter word to look for,or q to quit:";
		string s;
		//若遇到文件尾或用户输入了q时循环终止
		if (!(cin >> s) || s == "q") break;
		print(cout, tq.query(s)) << endl;

	}
}

int main(int argc,char **argv)
{
	ifstream infile;

	if (argc < 2 || !(infile.open(argv[1]), infile)) {
		cerr << "No input file!" << endl;
		return EXIT_FAILURE;
	}

	runQueries(infile);

	system("pause");
	return 0;
}