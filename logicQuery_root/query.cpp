#include "query.h"
#include "TextQuery.h"

#include <memory>
using std::shared_ptr;

#include <set>
using std::set;

#include <algorithm>
using std::set_intersection;

#include <iostream>
using std::ostream;

#include <cstddef>
using std::size_t;

#include <iterator>
using std::iterator;  //迭代器

#include <vector>
using std::vector;

#include <string>
using std::string;


//返回运算对象的结果set中不存在的行
QueryResult
NotQuery::eval(const TextQuery& text) const
{
	QueryResult result = query.eval(text);  //NotQuery类中定义了query成员

	shared_ptr<set<line_no>> ret_lines(new set<line_no>);

	QueryResult::line_it beg = result.begin(), end = result.end();
	
	//std::shared_ptr<std::vector<std::string>> get_file() { return file; }
	//get_file()定义在QueryResult.h中 返回一个shared_ptr
	//对于输入文件的每一行，如果改行不在result中，则将其添加到ret_lines
	vector<string>::size_type sz = result.get_file()->size();
	for (size_t n = 0; n != sz; ++n)
	{
		if (beg == end || *beg != n)
			ret_lines->insert(n);
		else if (beg != end)
			++beg;
	}
	
	return QueryResult(rep(), ret_lines, result.get_file());
}


//返回运算对象查询结果set的并集
QueryResult
AndQuery::eval(const TextQuery& text) const
{
	QueryResult left = lhs.eval(text), right = rhs.eval(text);
	//保存left和right的交集
	//shared_ptr<set<line_no>> ret_lines = make_shared<set<line_no>>(); 等价语句 
	shared_ptr<set<line_no>> ret_lines(new set<line_no>);

	set_intersection(left.begin(), left.end(),
		right.begin(), right.end(),
		inserter(*ret_lines, ret_lines->begin()));
	return QueryResult(rep(), ret_lines, left.get_file());
}

//返回运算对象查询结果set的并集
QueryResult
OrQuery::eval(const TextQuery& text) const
{
	QueryResult right = rhs.eval(text), left = lhs.eval(text);
	//将左侧运算对象的行号拷贝到结果set中
	shared_ptr<set<line_no>> ret_lines(new set<line_no>(left.begin(), left.end()));

	ret_lines->insert(right.begin(), right.end());

	return QueryResult(rep(), ret_lines, left.get_file());
}


