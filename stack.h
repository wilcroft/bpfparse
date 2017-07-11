#pragma once
#include <string>
#include <vector>
#include <list>
#include "istring.h"

struct stack {
	// Finished predicates
	std::list <std::list<std::string>> preds;
	// Active predicate(s) for processing
	std::list <std::list<std::string>> active;

	void addpred(std::string s);
	void newpred();

};

struct stacks {

	std::vector <stack> st;

	stacks();
	void open();
	void close();
	void addpred(std::string s);
	void newpred();

};