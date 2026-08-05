#include <gtest/gtest.h>

#include "Utils.hpp"

namespace {

struct MatchParam {
	const char* str;
	const char* filter;
	bool result;
};

class MatchTest
	: public testing::TestWithParam<MatchParam> {
};

TEST_P(MatchTest, Test) {
	const auto& param = GetParam();
	Storage storage;
	const bool result = match(
		param.str, strlen(param.str),
		param.filter, strlen(param.filter),
		storage
	);

	if (param.result) {
		EXPECT_TRUE(result);
	}
	else {
		EXPECT_FALSE(result);
	}
}

INSTANTIATE_TEST_SUITE_P(MatchTest,
	MatchTest,
	testing::Values(
		MatchParam{ "", "", true },
		MatchParam{ "s", "s", true },
		MatchParam{ "str", "str", true },
		MatchParam{ "str", "s*", true },
		MatchParam{ "str", "st*", true },
		MatchParam{ "str", "st?", true },
		MatchParam{ "some str some", "*str*", true },
		MatchParam{ "str", "???", true },
		MatchParam{ "some long string", "*", true },
		MatchParam{ "aa", "??*", true },

		MatchParam{ "a", "??*", false},
		MatchParam{ "str", "", false },
		MatchParam{ "str", "r", false },
		MatchParam{ "str", "?", false },
		MatchParam{ "str", "??", false }
	));

} // namespace