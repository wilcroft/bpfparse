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
	std::list<istring::istring> items = istring::tokenize(s);

	for (std::list<istring::istring>::iterator it = items.begin(); it != items.end(); it++) {
		// Separate grouped/unspaced brackets
		if (*it != "(" && it->find("(") != std::string::npos) {
			std::list<istring::istring> insert;
			size_t pos = it->find("(");
			if (pos != 0) insert.emplace_back(it->substr(0, pos));
			insert.emplace_back("(");
			if (pos+1 != it->size()) insert.emplace_back(it->substr(pos + 1));
			it = istring::replace(items, it, insert);
		}
		else if (*it != ")" && it->find(")") != std::string::npos) {
			std::list<istring::istring> insert;
			size_t pos = it->find(")");
			if (pos != 0) insert.emplace_back(it->substr(0, pos));
			insert.emplace_back(")");
			if (pos+1 != it->size()) insert.emplace_back(it->substr(pos + 1));
			it = istring::replace(items, it, insert);
		}
		// expand ethernet protocol type
		else if ((*it == "ip" || *it == "ip6" || *it == "arp" || *it == "rarp")
					&& (it == items.begin() || *((--it)++) != "proto") 
					&& (it == items.end() || *((++it)--) != "proto" ) ) {
			std::list<istring::istring> toinsert;
			toinsert.push_back("ether");
			toinsert.push_back("proto");
			toinsert.push_back(*it);
			it = istring::replace(items, it, toinsert);
		}
		// expand ip protocol type
		else if ((*it == "tcp" || *it == "udp" || *it == "icmp")
			&& (it == items.begin() || *((--it)++) != "proto")
			&& (it == items.end() || *((++it)--) != "proto")) {
			std::list<istring::istring> toinsert;
			toinsert.push_back("(");
			toinsert.push_back("ip");
			toinsert.push_back("proto");
			toinsert.push_back(*it);
			toinsert.push_back("or");
			toinsert.push_back("ip6");
			toinsert.push_back("proto");
			toinsert.push_back(*it);
			toinsert.push_back(")");
			it = istring::replace(items, it, toinsert);
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
