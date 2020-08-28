#include <gtest/gtest.h>

#include "testbiginteger.h"

template<typename T, std::size_t ...I>
void test_helper(std::index_sequence<I...>)
{
	constexpr T one = 1u;
	static_assert(((one << I == T::exp(2u,I)) && ...), "Left shift failed");
}


TYPED_TEST(BigIntegerTests, BitShiftLeft)
{
	/// max number
	TypeParam val = TypeParam::max();
	TypeParam exp_val = TypeParam::max();
	for(std::size_t i = 0; i < TypeParam::bit_size; i++) {
		EXPECT_EQ(val << i, exp_val);
		exp_val *= 2u;
	}

	///  arbitrary number
	val = 100000000lu;
	exp_val = 100000000lu;
	for(std::size_t i = 0; i < TypeParam::bit_size; i++) {
		EXPECT_EQ(val << i, exp_val);
		exp_val *= 2u;
	}

	/// zero
	TypeParam zero = 0u;
	for(std::size_t i = 0; i < TypeParam::bit_size; i++)
	{
		EXPECT_EQ(zero << i, 0u);
	}

	// operator<<=
	val = 100000000lu;
	exp_val = 100000000lu;
	for(std::size_t i = 0; i < TypeParam::bit_size; i++){ // 1 is already a bit
		EXPECT_EQ(val  <<= 1u, exp_val <<= 1u);
	}

	for(std::size_t i = 0; i < TypeParam::bit_size; i++){
		EXPECT_EQ(zero <<= 1u, 0u);
	}

	/// Test compile time computation
	test_helper<TypeParam>(std::make_index_sequence<5>{});
}

TYPED_TEST(BigIntegerTests, BitShiftRight)
{
	///  max number
	TypeParam val = TypeParam::max();
	TypeParam exp_val = TypeParam::max();
	for(std::size_t i = 0; i < TypeParam::bit_size; i++) {
		EXPECT_EQ(val >> i, exp_val);
		exp_val /= 2u;
	}

	///  arbitrary number
	val = TypeParam::max() - 100000000lu;
	exp_val = TypeParam::max() - 100000000lu;
	for(std::size_t i = 0; i < TypeParam::bit_size; i++) {
		EXPECT_EQ(val >> i, exp_val);
		exp_val /= 2u;
	}
}
