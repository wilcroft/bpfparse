#include "istring.h"
#include <iostream>

int main() {

	istring::istring i1 ("foo");
	istring::istring i2("foO1");
	istring::istring i3("Foo1");
	std::string s1("FOO");
	std::cout << "s:" << s1 << std::endl;
	std::cout << "i:" << i1 << std::endl;
	std::cout << (s1 == (std::string)i1 ? "true" : "false") << std::endl;
	std::cout << (i1 == s1 ? "true" : "false") << std::endl;
	std::cout << (i2 == i3 ? "true" : "false") << std::endl;
	std::cout << (i3 == i2 ? "true" : "false") << std::endl;

	std::list<istring::istring> l = istring::tokenize("Hi There   friends\t! ", istring::ws);
	for (auto&s : l) std::cout << s << "+";
	std::cout << std::endl; 
	l = istring::tokenize("Ifoomissedfooyoufoofoo!", "foo");
	for (auto&s : l) std::cout << s << "=";
	std::cout << std::endl;
	l = istring::tokenize("192.168.0.145", ".");
	for (auto&s : l) std::cout << s << "=";
	std::cout << std::endl;
	return 0;
}