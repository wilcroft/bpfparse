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
private:
	std::vector <stack> st;

public:
	std::vector <std::list<std::string>> preds;

	stacks();
	void open();
	void close();
	void addpred(std::string s);
	void newpred();

};