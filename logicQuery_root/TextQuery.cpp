#include "TextQuery.h"
#include "make_plural.h"

#include <cstddef>
#include <memory>
#include <sstream>  //istringstream && ostringstream
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <cctype>  //ispunct() tolower()
#include <utility>
#include <cstring>


using std::size_t;
using std::shared_ptr;
using std::istringstream;
using std::string;
using std::getline; //?
using std::vector;
using std::set;
using std::map;
using std::set;
using std::cerr;
using std::cout;
using std::cin;
using std::ostream;
using std::endl;
using std::ifstream;
using std::ispunct; //�Ƿ��Ǳ����� ΪʲôҪ����
using std::tolower;
using std::strlen;
using std::pair;


//û��ʹ��auto ʹ��typedef������
//std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
// key->std::string , value->std::shared_ptr<std::set<line_no>>

// using line_no = std::vector<std::string>::size_type;

typedef map<string, shared_ptr<set<TextQuery::line_no>>> wmType;

typedef wmType::const_iterator wmIter;

typedef shared_ptr<set<TextQuery::line_no>> lineType;

typedef set<TextQuery::line_no>::const_iterator lineIter;

//���캯�� ����һ��ifstream ���ж�ȡ�ļ�
TextQuery::TextQuery(ifstream &is) :file(new vector<string>)
{
	string text;
	while (getline(is, text))
	{
		file->push_back(text);
		int n = file->size() - 1;  //�� ��print()����
		istringstream line(text);
		string word;
		while (line >> word)
		{
			word = cleanup_str(word);
			//�±���� ���عؼ���Ϊword��Ԫ�أ�std::shared_ptr<std::set<line_no>����
			//������ʣ�word������wm�У��Դ˵���Ϊ�±���ӵ�wm�У���word����ֵ��ʼ��
			lineType &lines = wm[word]; //lines��һ��shared_ptr
										//�ڵ�һ�������������ʱ����ָ��Ϊ��
			if (!lines)
			{
				lines.reset(new set<line_no>);
			}
			lines->insert(n);
		}
	}
}

//�Ƴ��������У����б����� ��������ת��ΪСд 
string TextQuery::cleanup_str(const string &word)
{
	string ret;
	for (string::const_iterator it = word.begin();
	it != word.end(); ++it)
	{
		if (!ispunct(*it))
		{
			ret += tolower(*it);
		}
	}
	return ret;
}

QueryResult TextQuery::query(const string &sought) const
{
	//���û���ҵ�sought ����һ��ָ���set��ָ��
	static lineType nodata(new set<line_no>);
	//ʹ��find�������±�����������ҵ��ʣ����⽫������ӵ�wm��
	wmIter loc = wm.find(cleanup_str(sought));

	if (loc == wm.end())
		return QueryResult(sought, nodata, file);
	else
		return QueryResult(sought, loc->second, file); //ֱ�ӷ��� ���ٿ����ɱ�

}

//��ӡ���
ostream &print(ostream & os, const QueryResult &qr)
{
	//����ҵ��˵��ʣ���ӡ�����������г��ֵ�λ��
	os << qr.sought << " occurs " << qr.lines->size() << " "
		<< make_plural(qr.lines->size(), "times", "s") << endl;

	//��ӡ���ʳ��ֵ�ÿһ��
	for (lineIter num = qr.lines->begin(); num != qr.lines->end(); ++num)
	{
		os << "\t(line " << *num + 1 << ") "
			<< *(qr.file->begin() + *num) << endl;
	}
	return os;
}

//�������� ��ӡmap û���õ�display_map
//typedef wmType::const_iterator wmIter;
//std::map<std::string, std::shared_ptr<std::set<line_no>>> wm; wm�Ƕ��� ���������ͱ���
//typedef shared_ptr<set<TextQuery::line_no>> lineType;
//typedef set<TextQuery::line_no>::const_iterator lineIter;
void TextQuery::display_map()
{
	wmIter iter = wm.begin(), iter_end = wm.end();
	for (; iter != iter_end; ++iter)
	{
		cout << "word: " << iter->first << "{";

		lineType text_locs = iter->second;
		lineIter loc_iter = text_locs->begin(),
			loc_iter_end = text_locs->end();

		while (loc_iter != loc_iter_end)
		{
			cout << *loc_iter;

			if (++loc_iter != loc_iter_end)
				cout << ", ";
		}

		cout << "}\n";
	}
	cout << endl;
}



