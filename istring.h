#pragma once
#include <string>
#include <list>
#include <locale>

namespace istring {
	using std::string;

	class istring : public virtual std::string {
	public:

		bool operator==(string s);
//		bool operator==(const char* c);
		bool operator==(const char c[]);
		istring();
		istring(std::string s);
		istring(char cp[]);
		istring(std::string s, size_t pos, size_t len = npos);
//		istring(char c);
		size_t ifind(string s, size_t npos = 0);

	};

	static istring ws;

	string toLower(string s);
	std::list<istring> tokenize(istring s, istring token = ws);
	std::list<istring>::iterator replace(std::list<istring>& s, std::list<istring>::iterator it, std::list<istring> r);
}