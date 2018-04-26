#include <iostream> //iostream�����˶�д���Ļ�������
#include <fstream> //fstream�����˶�д�����ļ�������

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


//tip:������ʵ�ֳ�Ա֮ǰ�ȱ�д����ʹ������� �������Կ������Ƿ��������Ҫ�Ĳ���
void runQueries(ifstream &infile)
{
	//infile��һ��ifstream��ָ��Ҫ������ļ�
	TextQuery tq(infile); //�����ļ���������ѯmap [�õ����캯�� TextQuery::TextQuery(ifstream &is) :file(new vector<string>)]
	//���û���������ʾ�û�����Ҫ��ѯ�ĵ��ʣ���ɲ�ѯ����ӡ���
	while (true) {
		cout << "enter word to look for,or q to quit:";
		string s;
		//�������ļ�β���û�������qʱѭ����ֹ
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