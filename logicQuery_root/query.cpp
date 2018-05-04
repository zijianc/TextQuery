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
using std::iterator;  //������

#include <vector>
using std::vector;

#include <string>
using std::string;


//�����������Ľ��set�в����ڵ���
QueryResult
NotQuery::eval(const TextQuery& text) const
{
	QueryResult result = query.eval(text);  //NotQuery���ж�����query��Ա

	shared_ptr<set<line_no>> ret_lines(new set<line_no>);

	QueryResult::line_it beg = result.begin(), end = result.end();
	
	//std::shared_ptr<std::vector<std::string>> get_file() { return file; }
	//get_file()������QueryResult.h�� ����һ��shared_ptr
	//���������ļ���ÿһ�У�������в���result�У�������ӵ�ret_lines
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


//������������ѯ���set�Ĳ���
QueryResult
AndQuery::eval(const TextQuery& text) const
{
	QueryResult left = lhs.eval(text), right = rhs.eval(text);
	//����left��right�Ľ���
	//shared_ptr<set<line_no>> ret_lines = make_shared<set<line_no>>(); �ȼ���� 
	shared_ptr<set<line_no>> ret_lines(new set<line_no>);

	set_intersection(left.begin(), left.end(),
		right.begin(), right.end(),
		inserter(*ret_lines, ret_lines->begin()));
	return QueryResult(rep(), ret_lines, left.get_file());
}

//������������ѯ���set�Ĳ���
QueryResult
OrQuery::eval(const TextQuery& text) const
{
	QueryResult right = rhs.eval(text), left = lhs.eval(text);
	//��������������кſ��������set��
	shared_ptr<set<line_no>> ret_lines(new set<line_no>(left.begin(), left.end()));

	ret_lines->insert(right.begin(), right.end());

	return QueryResult(rep(), ret_lines, left.get_file());
}


