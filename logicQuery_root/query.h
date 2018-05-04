#ifndef QUERY_H
#define QUERY_H
#include "TextQuery.h"
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>


class Query_base
{
	friend class Query;

protected:
	typedef TextQuery::line_no line_no;//用于eval函数
	virtual ~Query_base() {}

private:
	virtual QueryResult eval(const TextQuery&) const = 0;
	virtual std::string rep() const = 0;

};

//定义Query接口类，负责隐藏整个继承体系
class Query {
	//运算符要访问shared_ptr的构造函数，而该函数是私有的
	friend Query operator~(const Query &);
	friend Query operator|(const Query&, const Query&);
	friend Query operator&(const Query&, const Query&);

public:
	Query(const std::string&);  //构建一个新的WordQuery类 稍后定义此构造函数1
	QueryResult eval(const TextQuery &t) const
	{
		return q->eval(t);
	}
	std::string rep() const { return q->rep(); } //虚调用
private:
	Query(std::shared_ptr<Query_base> query): q(query){ } //构造函数2
	std::shared_ptr<Query_base> q;

};

//重载<<运算符
inline
std::ostream &
operator << (std::ostream &os, const Query &query)
{
	//Query::rep通过它的Query_base指针 对rep()进行了虚调用
	return os << query.rep();
}


class WordQuery : public Query_base {
	friend class Query;  //Query类使用了WordQuery类的构造函数
	WordQuery(const std::string &s) :query_word(s){ }

	QueryResult eval(const TextQuery &t) const
	{
		return t.query(query_word);
	}

	std::string rep() const { return query_word; }
	std::string query_word;
};

//定义Query的构造函数1
//此构造函数接受分配一个WordQuery，然后另其指针成员（p）指向新分配的成员
inline
Query::Query(const std::string &s) :q(new WordQuery(s)){ }

class NotQuery :public Query_base {
	friend Query operator~(const Query &);
	NotQuery(const Query &q):query(q){ }

	std::string rep() const { return "~(" + query.rep() + ")"; }
	QueryResult eval(const TextQuery&) const;
	Query query;

};

inline Query operator~(const Query &operand)
{
	return std::shared_ptr<Query_base>(new NotQuery(operand));
}

class BinaryQuery :public Query_base {
protected:
	BinaryQuery(const Query &l,const Query &r,std::string s):
		lhs(l),rhs(r),opSym(s) { }
	std::string rep() const {
		return "(" + lhs.rep() + " "
			+ opSym + " "
			+ rhs.rep() + ")";
	}

	Query lhs, rhs;
	std::string opSym;
};

class AndQuery :public BinaryQuery {
	friend Query operator&(const Query&, const Query&);
	AndQuery(const Query &left, const Query &right) :
		BinaryQuery(left, right, "&") { }

	//具体的类：AndQuery继承了rep并定义纯虚函数eval()
	QueryResult eval(const TextQuery&) const;
};

inline
Query operator&(const Query &lhs, const Query &rhs) {
	return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}


class OrQuery :public BinaryQuery {
	friend Query operator|(const Query&, const Query&);
	OrQuery(const Query &left,const Query &right):
		BinaryQuery(left,right,"|"){ }
	QueryResult eval(const TextQuery&) const;
};

inline 
Query  operator|(const Query &lhs, const Query &rhs)
{
	return std::shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}

std::ifstream& open_file(std::ifstream&, const std::string&);
TextQuery get_file(int, char**);
bool get_word(std::string&);
bool get_words(std::string&, std::string&);
std::ostream &print(std::ostream&, const QueryResult&);

#endif

