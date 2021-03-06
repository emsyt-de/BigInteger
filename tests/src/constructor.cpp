/*
 * This file is part of the XXX distribution (https://github.com/xxxx).
 * Copyright (c) 2020 Martin Schuler.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the free Software foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or fITNESS fOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "testbiginteger.h"

TYPED_TEST(BigIntegerTests, StandardConstructor)
{
	TypeParam  a;
	EXPECT_EQ(a,0u);
}

TYPED_TEST(BigIntegerTests, LiteralConstructor)
{
	if constexpr(TypeParam::bit_size == 128)
	{
		using namespace uint128;
		constexpr TypeParam a = 0xffffffffffffffffffffffffffffffff_num;
		constexpr TypeParam b = 0777777777777777777777777777777777777777777_num;
		constexpr TypeParam c = 85070591730234615865843651857942052863_num;
		ASSERT_EQ(a,TypeParam::max());
		ASSERT_EQ(b,c);
	}
	else if constexpr(TypeParam::bit_size == 256)
	{
		using namespace uint256;
		constexpr TypeParam a = 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff_num;
		constexpr TypeParam b = 07777777777777777777777777777777777777777777777777777777777777777777777777777777777777_num;
		constexpr TypeParam c = 57896044618658097711785492504343953926634992332820282019728792003956564819967_num;
		ASSERT_EQ(a,TypeParam::max());
		ASSERT_EQ(b,c);
	}
	else if constexpr(TypeParam::bit_size == 512)
	{
		if constexpr(std::is_unsigned_v<TypeParam>)
		{
			using namespace uint512;
			constexpr TypeParam a = 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff_num;
			constexpr TypeParam b = 077777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777_num;
			constexpr TypeParam c = 3351951982485649274893506249551461531869841455148098344430890360930441007518386744200468574541725856922507964546621512713438470702986642486608412251521023_num;
			ASSERT_EQ(a,TypeParam::max());
			ASSERT_EQ(b,c);
		}
		else if constexpr(std::is_signed_v<TypeParam>)
		{
			using namespace int512;
			constexpr TypeParam a = 0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff_num;
			constexpr TypeParam b = 077777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777_num;
			constexpr TypeParam c = 3351951982485649274893506249551461531869841455148098344430890360930441007518386744200468574541725856922507964546621512713438470702986642486608412251521023_num;
			ASSERT_EQ(a,TypeParam::max());
			ASSERT_EQ(b,c);
		}
	}
	else if constexpr(TypeParam::bit_size == 1024)
	{
		using namespace uint1024;
		constexpr TypeParam a = 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff_num;
		constexpr TypeParam b = 077777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777_num;
		constexpr TypeParam c = 89884656743115795386465259539451236680898848947115328636715040578866337902750481566354238661203768010560056939935696678829394884407208311246423715319737062188883946712432742638151109800623047059726541476042502884419075341171231440736956555270413618581675255342293149119973622969239858152417678164812112068607_num;
		ASSERT_EQ(a,TypeParam::max());
		ASSERT_EQ(b,c);
	}
	else
	{
		/// Expression is always false but depends on bit_size
		static_assert (!(TypeParam::bit_size || !TypeParam::bit_size), "Unsupported size type");
	}
}

