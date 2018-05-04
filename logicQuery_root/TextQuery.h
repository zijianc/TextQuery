#pragma once
#ifndef TEXTQUERY_H
#define TEXTQUERY_H

#include <vector>
#include <string>
#include <map>
#include <set>
#include <memory>  //智能指针
#include "QueryResult.h"

class QueryResult; //query函数会用到QueryResult类

class TextQuery {
public:
	using line_no = std::vector<std::string>::size_type;
	TextQuery(std::ifstream&);
	QueryResult query(const std::string&) const;

	void display_map();  //调试帮助：打印map
private:
	std::shared_ptr<std::vector<std::string>> file;
	std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;

	//移除所有标点符号，并把所有单词改为小写
	static std::string cleanup_str(const std::string&);
};



#endif