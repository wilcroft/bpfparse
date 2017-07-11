#include "rule.h"

void BpfRule::addRule(enum BPFproto p, enum BPFdir d, enum BPFtype t, int v){
	predlist.emplace_back(p,d,t,v);
}

void BpfRule::addRule(istring::istring s) {

/*	std::list<std::string> subrules;
	size_t pos;
	while ((pos = s.ifind(" and ")) != std::string::npos) {
		subrules.push_back(s.substr(0, pos));
		s.erase(0, pos+5);
	}
	subrules.push_back(s);

	for (auto x : subrules) {
		std::cout << x << std::endl;
	}
*/
	std::list<std::string> items = istring::tokenize(s);

	for (std::list<std::string>::iterator it = items.begin(); it != items.end(); it++) {
		if (*it != "(" && it->find("(") != std::string::npos) {
			std::list<std::string>::iterator old = it;
			it++;
			std::list<std::string> insert;
			size_t pos = old->find("(");
			if (pos != 0) insert.emplace_back(old->substr(0, pos));
			insert.emplace_back("(");
			if (pos+1 != old->size()) insert.emplace_back(old->substr(pos + 1));
			items.erase(old);
			items.insert(it, insert.begin(), insert.end());
			it--;
			it--;
		}
		else if (*it != ")" && it->find(")") != std::string::npos) {
			std::list<std::string>::iterator old = it;
			it++;
			std::list<std::string> insert;
			size_t pos = old->find(")");
			if (pos != 0) insert.emplace_back(old->substr(0, pos));
			insert.emplace_back(")");
			if (pos+1 != old->size()) insert.emplace_back(old->substr(pos + 1));
			items.erase(old);
			items.insert(it, insert.begin(), insert.end());
			it--;
			it--;
		}

	}

	struct stacks stk;

	for (auto &i : items) {
		if (i == "(") {
			stk.open();
		}
		else if (i == ")") {
			stk.close();
		}
	//	if (i == "and") {
//
	//	}
		else if (i == "or") {
			stk.newpred();
		}
		else stk.addpred(i);
	}

	for (auto& i : stk.st[0].preds) {
		for (auto& str : i) {
			std::cout << str << " ";
		}
		std::cout << std::endl;
	}
	for (auto& i : stk.st[0].active) {
		for (auto& str : i) {
			std::cout << str << " ";
		}
		std::cout << std::endl;
	}

}
