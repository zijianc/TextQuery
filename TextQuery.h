#pragma once
#ifndef TEXTQUERY_H
#define TEXTQUERY_H

#include <vector>
#include <string>
#include <map>
#include <set>
#include <memory>  //����ָ��
#include "QueryResult.h"

class QueryResult; //query�������õ�QueryResult��

class TextQuery {
public:
	using line_no = std::vector<std::string>::size_type;
	TextQuery(std::ifstream&);
	QueryResult query(const std::string&) const;

	void display_map();  //���԰�������ӡmap
private:
	std::shared_ptr<std::vector<std::string>> file;
	std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;

	//�Ƴ����б����ţ��������е��ʸ�ΪСд
	static std::string cleanup_str(const std::string&);
};



#endif