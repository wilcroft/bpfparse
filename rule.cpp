#include "rule.h"

void BpfRule::addRule(enum BPFproto p, enum BPFdir d, enum BPFtype t, int v){
	predlist.emplace_back();// p, d, t, v);
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
		// expand undirected net 
		else if ((*it == "net")	&& (it == items.begin() || *((--it)++) != "src" && *((--it)++) != "dst")) {
			std::list<istring::istring> toinsert;
			toinsert.push_back("(");
			toinsert.push_back("src");
			toinsert.push_back("net");
			toinsert.push_back("or");
			toinsert.push_back("dst");
			toinsert.push_back("net");
			toinsert.push_back(")");
//			toinsert.push_back(*it);
			it = istring::replace(items, it, toinsert);
		}
		// expand ip protocol type
		else if ((*it == "tcp" || *it == "udp" || *it == "icmp")
					&& (it == items.begin() || *((--it)++) != "proto")
					&& (it == items.end() || *((++it)--) != "proto")) {
			std::list<istring::istring> toinsert;
//			toinsert.push_back("(");
			toinsert.push_back("ip");
			toinsert.push_back("proto");
			toinsert.push_back(*it);
//			toinsert.push_back("or");
//			toinsert.push_back("ip6");
//			toinsert.push_back("proto");
//			toinsert.push_back(*it);
//			toinsert.push_back(")");
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

//	std::cout << "#:" << stk.preds.size() << std::endl;
	int idx=0;
// Parse the terms
	for (auto &i : stk.preds) {
		if (stk.preds.size() > 1) std::cout << "found " << stk.preds.size() << std::endl;
		std::string predstr ="";
		for (auto& str : i) {
			//std::cout << str << " ";
			predstr += str + " ";
		}
		//std::cout << std::endl;

		BpfPredicate pred;
		state st;
		pegtl::string_input<> rule_in(predstr, "s"+idx);
		try {
			//pegtl::parse< ruleparse::ipmatchold, ruleparse::action>(rule_in, pred, st);
			pegtl::parse< ruleparse::findmatch, ruleparse::action>(rule_in, pred, st);
		}
		catch (pegtl::parse_error& pe) {
			std::cerr << pe.what();
			std::cerr << std::endl;
		}
		if (pred.dir == BPFdirOR) {
			BpfPredicate pred2 = pred;
			std::string star = "*";
			pred2.dir = BPFdirAND;
			pred.dir = BPFdirAND;
			pred.addSrcPort(star);
			pred2.addDestPort(star);
			predlist.push_back(pred2);
		}
		predlist.push_back(pred);
		idx++;
		//std::list<std::string> preds;
		//preds.emplace_back("");
		//for (auto& str : i) {
		//	if ((istring::istring)str == "and") preds.emplace_back("");
		//	else preds.back() = preds.back() + " " + str;
		//}
		//if (preds.back() == "") {
		//	std::cerr << "Unexpected Error in rule parsing! Exiting..." << std::endl;
		//	exit(-1);
		//}
		//
	}

}

void BpfRule::writeRulesToFile(std::string fname) {
	std::ofstream ofs;

	ofs.open(fname, std::ofstream::out);

	if (!ofs.is_open()) {
		std::string msg = "Couldn't open file " + fname + " for writing.";
		throw std::exception(msg.c_str());
	}

	int i = predlist.front().tobits().length();
	ofs << ".i " << i << std::endl;
	ofs << ".o " << 1 << std::endl;
	ofs << ".type fdr" << std::endl;

	for (auto& p : predlist)
		ofs << p.tobits() << "\t1" << std::endl;

	ofs.close();
}
