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
	typedef TextQuery::line_no line_no;//����eval����
	virtual ~Query_base() {}

private:
	virtual QueryResult eval(const TextQuery&) const = 0;
	virtual std::string rep() const = 0;

};

//����Query�ӿ��࣬�������������̳���ϵ
class Query {
	//�����Ҫ����shared_ptr�Ĺ��캯�������ú�����˽�е�
	friend Query operator~(const Query &);
	friend Query operator|(const Query&, const Query&);
	friend Query operator&(const Query&, const Query&);

public:
	Query(const std::string&);  //����һ���µ�WordQuery�� �Ժ���˹��캯��1
	QueryResult eval(const TextQuery &t) const
	{
		return q->eval(t);
	}
	std::string rep() const { return q->rep(); } //�����
private:
	Query(std::shared_ptr<Query_base> query): q(query){ } //���캯��2
	std::shared_ptr<Query_base> q;

};

//����<<�����
inline
std::ostream &
operator << (std::ostream &os, const Query &query)
{
	//Query::repͨ������Query_baseָ�� ��rep()�����������
	return os << query.rep();
}


class WordQuery : public Query_base {
	friend class Query;  //Query��ʹ����WordQuery��Ĺ��캯��
	WordQuery(const std::string &s) :query_word(s){ }

	QueryResult eval(const TextQuery &t) const
	{
		return t.query(query_word);
	}

	std::string rep() const { return query_word; }
	std::string query_word;
};

//����Query�Ĺ��캯��1
//�˹��캯�����ܷ���һ��WordQuery��Ȼ������ָ���Ա��p��ָ���·���ĳ�Ա
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

	//������ࣺAndQuery�̳���rep�����崿�麯��eval()
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

