#include <gtest/gtest.h>

#include "Utils.hpp"

namespace {

struct RemoveDublicatesParam {
	const char* rawFilter;
	const char* result;
};

class RemoveDublicatesTest
	: public testing::TestWithParam<RemoveDublicatesParam> {
};

TEST_P(RemoveDublicatesTest, Test) {
	const auto& param = GetParam();
	char result[MAX_FILTER_LENGTH + 1];
	size_t resultLen = CopyAndRemoveDublicates(param.rawFilter, result);
	EXPECT_STREQ(param.result, result);
	EXPECT_EQ(strlen(result), resultLen) << "input \"" << param.rawFilter;
}

INSTANTIATE_TEST_SUITE_P(RemoveDublicatesTest,
	RemoveDublicatesTest,
	testing::Values(
		RemoveDublicatesParam{ "", "" },
		RemoveDublicatesParam{ "*", "*" },
		RemoveDublicatesParam{ "**", "*" },
		RemoveDublicatesParam{ "***", "*" },
		RemoveDublicatesParam{ "*a", "*a" },
		RemoveDublicatesParam{ "***a", "*a" },
		RemoveDublicatesParam{ "***bla", "*bla" },
		RemoveDublicatesParam{ "a*b", "a*b" },
		RemoveDublicatesParam{ "a*****b", "a*b" },
		RemoveDublicatesParam{ "blas***slab", "blas*slab" },
		RemoveDublicatesParam{ "a*", "a*" },
		RemoveDublicatesParam{ "a****", "a*" },
		RemoveDublicatesParam{ "blabla", "blabla" }
	));

} // namespace