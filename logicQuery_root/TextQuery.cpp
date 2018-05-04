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
using std::ispunct; //是否是标点符号 为什么要导入
using std::tolower;
using std::strlen;
using std::pair;


//没有使用auto 使用typedef简化声明
//std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
// key->std::string , value->std::shared_ptr<std::set<line_no>>

// using line_no = std::vector<std::string>::size_type;

typedef map<string, shared_ptr<set<TextQuery::line_no>>> wmType;

typedef wmType::const_iterator wmIter;

typedef shared_ptr<set<TextQuery::line_no>> lineType;

typedef set<TextQuery::line_no>::const_iterator lineIter;

//构造函数 接受一个ifstream 逐行读取文件
TextQuery::TextQuery(ifstream &is) :file(new vector<string>)
{
	string text;
	while (getline(is, text))
	{
		file->push_back(text);
		int n = file->size() - 1;  //？ 看print()函数
		istringstream line(text);
		string word;
		while (line >> word)
		{
			word = cleanup_str(word);
			//下标操作 返回关键字为word的元素（std::shared_ptr<std::set<line_no>），
			//如果单词（word）不在wm中，以此单词为下标添加到wm中，对word进行值初始化
			lineType &lines = wm[word]; //lines是一个shared_ptr
										//在第一次遇到这个单词时，此指针为空
			if (!lines)
			{
				lines.reset(new set<line_no>);
			}
			lines->insert(n);
		}
	}
}

//移除（单词中）所有标点符号 并将单词转换为小写 
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
	//如果没有找到sought 返回一个指向此set的指针
	static lineType nodata(new set<line_no>);
	//使用find而不是下标运算符来查找单词，避免将单词添加到wm中
	wmIter loc = wm.find(cleanup_str(sought));

	if (loc == wm.end())
		return QueryResult(sought, nodata, file);
	else
		return QueryResult(sought, loc->second, file); //直接返回 减少拷贝成本

}

//打印结果
ostream &print(ostream & os, const QueryResult &qr)
{
	//如果找到了单词，打印出次数和所有出现的位置
	os << qr.sought << " occurs " << qr.lines->size() << " "
		<< make_plural(qr.lines->size(), "times", "s") << endl;

	//打印单词出现的每一行
	for (lineIter num = qr.lines->begin(); num != qr.lines->end(); ++num)
	{
		os << "\t(line " << *num + 1 << ") "
			<< *(qr.file->begin() + *num) << endl;
	}
	return os;
}

//帮助调试 打印map 没有用到display_map
//typedef wmType::const_iterator wmIter;
//std::map<std::string, std::shared_ptr<std::set<line_no>>> wm; wm是对象 其他是类型别名
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



