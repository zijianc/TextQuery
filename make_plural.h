#pragma once
#include <cstddef> //size_t��ͷ�ļ�
using std::size_t;

#include <string>
using std::string;

#include <iostream>
using std::cout, using std::endl;

#ifndef MAKE_PLURAL_H
#define MAKE_PLURAL_H

inline
string make_plural(size_t ctr, const string &word, const string &ending)
{
	return (ctr > 1) ? word + ending : word;
}

#endif

