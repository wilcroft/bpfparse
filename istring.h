#pragma once
#include <string>
#include <list>
#include <locale>

namespace istring {
	using std::string;

	class istring : public virtual std::string {
	public:

		bool operator==(string s);
		istring();
		istring(std::string s);
		istring(char cp[]);
		istring(std::string s, size_t pos, size_t len = npos);
//		istring(char c);
		size_t ifind(string s, size_t npos = 0);

	};

	static istring ws;

	string toLower(string s);
	std::list<string> tokenize(istring s, istring token = ws);
}