#pragma once
#include <cstdint>

struct ternary {

	enum ternary_value : int_fast8_t {
		TERN_0,
		TERN_1,
		TERN_DC
	};

	enum ternary_value d;

	ternary();
	ternary(const enum ternary_value& t);
	bool operator==(const enum ternary_value& t);
	bool operator==(const ternary& t);
};