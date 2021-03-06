/*
 * This file is part of the XXX distribution (https://github.com/xxxx).
 * Copyright (c) 2020 Martin Schuler.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNu General Public license as published by
 * the free Software foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOuT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABIlITY or fITNESS fOR A PARTICulAR PuRPOSE. See the GNu
 * General Public license for more details.
 *
 * You should have received a copy of the GNu General Public license
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "testbiginteger.h"

static constexpr bool proof_const = false;

TYPED_TEST(BigIntegerTests, Multiply)
{
	TypeParam  a0,a1,a2;
	TypeParam  b0,b1,b2;
	TypeParam  c0,c1,c2;
	if constexpr(TypeParam::bit_size == 128)
	{
		using namespace uint128;
		a0 = 0xf_num;
		a1 = 0xffffffff_num;
		a2 = 0xff45d391f11421ae_num;
		b0 = 0x0_num;
		b1 = 0xffffffff_num;
		b2 = 0xff4b9c63cbd74d45_num;
		c1 = 0xfffffffe00000001_num;
		c2 = 0xfe91f3256b157e40e77a68c04bb069e6_num;

		if constexpr(proof_const)
		{
		/// Test compile time computing
		constexpr TypeParam ab3 = 0xff45d391f11421ae_num * 0xff4b9c63cbd74d45_num;
		constexpr TypeParam c3  = 0xfe91f3256b157e40e77a68c04bb069e6_num;
		static_assert (ab3 == c3, "Multiply failed");
		}
	}
	else if constexpr(TypeParam::bit_size == 256)
	{
		using namespace uint256;
		a0 = 0xf_num;
		a1 = 0xffffffff_num;
		a2 = 0xfffffffff_num;
		b0 = 0x0_num;
		b1 = 0xffffffff_num;
		b2 = 0xfffffffff_num;
		c1 = 0xfffffffe00000001_num;
		c2 = 0xffffffffe000000001_num;

		if constexpr(proof_const)
		{
		/// Test compile time computing
		constexpr TypeParam ab3 = 0xfffffffff_num*0xfffffffff_num;
		constexpr TypeParam c3  = 0xffffffffe000000001_num;
		static_assert (ab3 == c3, "Multiply failed");
		}
	}
	else if constexpr(TypeParam::bit_size == 512)
	{
		if constexpr(std::is_unsigned_v<TypeParam>)
		{
		using namespace uint512;
		a0 = 0xf_num;
		a1 = 0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffed_num;
		a2 = 0x346789abc346789abc346789abc346789abc346789abc346789abc34678_num;
		b0 = 0x0_num;
		b1 = 3u;
		b2 = 0x893ace7477dca3262672246789abc346789abc346789abc346789abc_num;
		c1 = 0x17fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffc7_num;
		c2 = 0x1c17726544a6d8d83608f44a918f2c2e136fe05177dc6cde00e81dbb63e5be587eacae577e2a2b1bc096ac5ee51bdc2f48e2e4d98a3740bf020_num;

		if constexpr(proof_const)
		{
		/// Test compile time computing
		constexpr TypeParam ab3 = 0x346789abc346789abc346789abc346789abc346789abc346789abc34678_num
				* 0x893ace7477dca3262672246789abc346789abc346789abc346789abc_num;
		constexpr TypeParam c3  = 0x1c17726544a6d8d83608f44a918f2c2e136fe05177dc6cde00e81dbb63e5be587eacae577e2a2b1bc096ac5ee51bdc2f48e2e4d98a3740bf020_num;
		static_assert (ab3 == c3, "Multiply failed");
		}
		}
	}
	else if constexpr(TypeParam::bit_size == 1024)
	{
		using namespace uint1024;
		a0 = 0xf_num;
		a1 = 0xffffffff_num;
		a2 = 0x346789abc346789abc34678234769ab4654c34678932dabc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346_num;
		b0 = 0x0_num;
		b1 = 0x346789abc346789abc346789abc346789abc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346789abc_num;
		b2 = 0x789abc346789ab8e0e81dbb63e5be577e2a2b1bc096ac5ee51bdc2f48e2e4d98a3740bf020346789ab8edeeeef289abc346789abc346789abc346789_num;
		c1 = 0x346789ab8edeeeeef8edeeeeef8edeeeeef8edeeeeef8edef21783321ef8edeeeeef8edeeeeef8edeeeeef8edeeeeef8edeeeeef8edeeeeef8b9876544_num;
		c2 = 0x18b035581830722386091c50d922d0779b4364b931ba579aa3cd50aadc768500498d1bc95f9ffb2d91685cdf47c1c7a705cd635cacae5ea65b7789ed126b8a8413c5d7025e4fd791011bbf02e68936c9c131c835c338b254a00dcd5a42df364a3cffba6ebfd2066d6b13e5ad895850cc41f01633dab4aa76_num;

		if constexpr(proof_const)
		{
		/// Test compile time computing
		constexpr TypeParam ab3 = 0x346789abc346789abc34678234769ab4654c34678932dabc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346_num
				* 0x789abc346789ab8e0e81dbb63e5be577e2a2b1bc096ac5ee51bdc2f48e2e4d98a3740bf020346789ab8edeeeef289abc346789abc346789abc346789_num;
		constexpr TypeParam c3  = 0x18b035581830722386091c50d922d0779b4364b931ba579aa3cd50aadc768500498d1bc95f9ffb2d91685cdf47c1c7a705cd635cacae5ea65b7789ed126b8a8413c5d7025e4fd791011bbf02e68936c9c131c835c338b254a00dcd5a42df364a3cffba6ebfd2066d6b13e5ad895850cc41f01633dab4aa76_num;
		static_assert (ab3 == c3, "Multiply failed");
		}
	}
	else
	{
		/// Expression is always false but depends on bit_size
		static_assert (!(TypeParam::bit_size || !TypeParam::bit_size), "Unsupported size type");
	}

	EXPECT_EQ(a0*b0, 0u);
	EXPECT_EQ(b0*a0, 0u);
	EXPECT_EQ(a1*b1, c1);
	EXPECT_EQ(b1*a1, c1);
	EXPECT_EQ(b2*a2, c2);
	EXPECT_EQ(a2*b2, c2);
}
