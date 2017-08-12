#include "istring.h"


namespace istring {
	istring::istring() {};
	istring::istring(std::string s) : string(s) { }// *this = s; };
	istring::istring(char cp[]) : string(cp) { }//*this = cp; };
	istring::istring(std::string s, size_t pos, size_t len) : string(s, pos, len) {  }
	//istring::istring::istring(char c): string (c) { };//*this = c; };

	bool istring::operator==(string s) {
		return toLower(s) == toLower(*this);
	}
	bool istring::operator==(const char c[]) {
		string s(c);
		return toLower(s) == toLower(*this);
	}
	bool istring::operator!=(string s) {
		return !operator==(s);
	}
	bool istring::operator!=(const char c[]) {
		return !operator==(c);
	}
	size_t istring::ifind(string s, size_t npos) {

		if (s.length() < npos) return string::npos;
		string t(s, npos);
		int i;
		for (i = 0; i <= (int)(length() - t.length()); i++) {
			istring seq(*this, i, t.length());
			if (seq == t) return i;
		}
		return string::npos;

	}

	// Returns a lowercase version of the given string
	std::string toLower(std::string s) {
		//for (auto c : s) {
		for (unsigned int i = 0; i < s.length(); i++) {
			if (s[i] < 91 && s[i] > 64)
				s[i] = s[i] + 32;
		}
		return s;
	}

	std::list<istring> tokenize(istring s, istring token) {
		std::list<istring> l;
		if (token == ws) {
			size_t pos;
			while (!s.empty()) {
				for (pos = 0; !isspace(s[pos]) && pos != s.size(); pos++);
				//pos = s.ifind(token);
				if (pos != 0)	l.push_back(s.substr(0, pos));
				s.erase(0, pos + 1);
			}
		}
		else {
			size_t pos;
			while (!s.empty()) {
				pos = s.ifind(token);
				if (pos != 0)	l.push_back(s.substr(0, pos));
				if (pos !=s.npos) s.erase(0, pos + token.size());
				else s.erase(0, s.npos);
			}
		}
		return l;
	}

// Replace the list item at it with the contents of list r
// Returns an iterator to the item
	std::list<istring>::iterator replace(std::list<istring>& s, std::list<istring>::iterator it, std::list<istring> r) {
		std::list<istring>::iterator iret;
		iret = s.insert(it, r.begin(), r.end());
		s.erase(it);
		return iret;
	}
}