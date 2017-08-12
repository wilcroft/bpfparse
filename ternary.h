#pragma once
#include <cstdint>
#include <vector>
#include <bitset>

namespace tern {

#define MAX_VECTOR_SIZE 48

class ternary {
public:
	enum ternary_value : int_fast8_t {
		TERN_0,
		TERN_1,
		TERN_DC
	};

	enum ternary_value d;

	ternary();
	ternary(const enum ternary_value& t);
	ternary(const bool& b);
	bool operator==(const enum ternary_value& t);
	bool operator==(const ternary& t) const;
	bool operator==(const bool& b);
	bool operator!=(const enum ternary_value& t);
	bool operator!=(const ternary& t);
	bool operator!=(const bool& b);
};

void setVector(std::vector<ternary>& v, int i);
}