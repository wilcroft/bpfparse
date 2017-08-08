#include "rule.h"

void BpfRule::addRule(enum BPFproto p, enum BPFdir d, enum BPFtype t, int v){
	predlist.emplace_back(p,d,t,v);
}

void BpfRule::addRule(istring::istring s) {

// First, we split the rule string into substrings on whitespace
	std::list<istring::istring> items = istring::tokenize(s);

// Next, we put brackets into separate string elements to make parsing easier
// We also expand some keywords that are short fords for longer expressions
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

// Now that we have the expanded form of the rule, we decompose it into individual terms
// That means decomposing the brackets and separating on ORs
// We use a stack to do this
	struct stacks stk;

	for (auto &i : items) {
		if (i == "(") {
			stk.open();
		}
		else if (i == ")") {
			stk.close();
		}
		else if (i == "or") {
			stk.newpred();
		}
		else stk.addpred(i);
	}
	stk.close();

// Parse the terms
	for (auto &i : stk.preds) {
		for (auto& str : i) {
			std::cout << str << " ";
		}
		std::cout << std::endl;

		BpfPredicate pred;
		std::list<std::string> preds;
		preds.emplace_back("");
		for (auto& str : i) {
			if ((istring::istring)str == "and") preds.emplace_back("");
			else preds.back() = preds.back() + " " + str;
		}
		if (preds.back() == "") {
			std::cerr << "Unexpected Error in rule parsing! Exiting..." << std::endl;
			exit(-1);
		}

	}

}
