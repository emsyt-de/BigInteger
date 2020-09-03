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

TYPED_TEST(BigIntegerTests, Subtract)
{
	TypeParam  a0,a1,a2;
	TypeParam  b0,b1,b2;
	TypeParam  c0,c1,c2;
	if constexpr(TypeParam::bit_size == 128)
	{
		using namespace uint128_ct;
		a0 = 0xf_num;
		a1 = 0xffffffff_num;
		a2 = 0xff45d391f11421ae1_num;
		b0 = 0x0_num;
		b1 = 0xffffffff_num;
		b2 = 0xff4b9c63cbd74d45_num;
		c0 = 0xfu;
		c1 = 0x0u;
		c2 = 0xef5119cbb456acd9c_num;

		if constexpr(proof_const)
		{
		/// Test compile time computing
		constexpr TypeParam ab3 = 0xff4b9c63cbd74d45_num - 0xff45d391f11421ae_num;
		constexpr TypeParam c3  = 0x5c8d1dac32b97_num;
		static_assert (ab3 == c3, "Minus failed");
		}
	}
	else if constexpr(TypeParam::bit_size == 256)
	{
		using namespace uint256_t;
		a0 = 0xf_num;
		a1 = 0xffffffff_num;
		a2 = 0xffffffffe0000000000000000000000001_num;
		b0 = 0x0_num;
		b1 = 0xffffffff_num;
		b2 = 0xfffffffffffffffffffffffff_num;
		c0 = 0xfu;
		c1 = 0x0u;
		c2 = 0xffffffffd0000000000000000000000002_num;

		if constexpr(proof_const)
		{
		/// Test compile time computing
		constexpr TypeParam ab3 = 0xffffffffe000000001fffffffff_num - 0xffffffffe000000001fffffffff_num;
		constexpr TypeParam c3  = 0x0u;
		static_assert (ab3 == c3, "Minus failed");
		}
	}
	else if constexpr(TypeParam::bit_size == 512)
	{
		using namespace uint512_t;
		a0 = 0x17fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffc4_num;
		a1 = 0x346789abc346789abc346789abc346789abc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346789abc_num;
		a2 = 0x1ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffb0_num;
		b0 = 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffda_num;
		b1 = 0x346789abc346789abc346789abc346789abc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346789abc_num;
		b2 = 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffda_num;
		c0 = 0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffea_num;
		c1 = 0x0u;
		c2 = 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd6_num;

		if constexpr(proof_const)
		{
		/// Test compile time computing
		constexpr TypeParam ab3 = 0x17fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffc4_num
				- 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffda_num;
		constexpr TypeParam c3  = 0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffea_num;
		static_assert (ab3 == c3, "Minus failed");
		}
	}
	else if constexpr(TypeParam::bit_size == 1024)
	{
		using namespace uint1024_t;
		a0 = 0xf_num;
		a1 = 0x346789abc346789abc34678234769ab4654c34678932dabc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346_num;
		a2 = 0x18b035581830722386091c50d922d0779b4364b931ba579aa3cd50aadc768500498d1bc95f9ffb2d91685cdf47c1c7a705cd635cacae5ea65b7789ed126b8a8413c5d7025e4fd791011bbf02e68936c9c131c835c338b254a00dcd5a42df364a3cffba6ebfd2066d6b13e5ad895850cc41f01633dab4aa76_num;
		b0 = 0x0_num;
		b1 = 0x789abc346789ab8e0e81dbb63e5be577e2a2b1bc789ab8edeeeef289abc346789abc346789abc346789_num;
		b2 = 0xad0245e02ad02428ca54832437827236dacb6433872d2802c47eee623929da0aa86254ca05174c95be9eca6ac549bf1356ed55789ab5d0245e02ad0245e02ad0245e02acf_num;
		c0 = 0xfu;
		c1 = 0x346789abc346789abc34678234769ab4654c2cdddd6f944399aea8c3a58b17dd88211c92094bc22217b799abcd0bcccd775bbcccd775bbcccd775bbd_num;
		c2 = 0x18b035581830722386091c50d922d0779b4364b931ba579aa3cd50aadc768500498d1bc95f9ffb2d91685cdf47c1c7a705cd6351dc8a00a3ae7547606d2358409b9eb394b19994588e493ed69e9a50a62e94278b3d1365b44e9903fe58f28f9de863c93950fcaee3bfb6e367a92d80a7e3ed693194d47fa7_num;

		if constexpr(proof_const)
		{
		/// Test compile time computing
		constexpr TypeParam ab3 = 0x789abc346789ab8e0e81dbb63e5be577e2a2b1bc096ac5ee51bdc2f48e2e4d98a3740bf020346789ab8edeeeef289abc346789abc346789abc346789_num
				- 0x346789abc346789abc34678234769ab4654c34678932dabc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346_num;
		constexpr TypeParam c3  = 0x44333288a44332f3524d743409e54ac37d567d548037eb321d563948cae7d1d55cfb7133ebccdddde848665432f4333288a44333288a44333288a443_num;
		static_assert (ab3 == c3, "Minus failed");
		}
	}
	else
	{
		/// Expression is always false but depends on bit_size
		static_assert (!(TypeParam::bit_size || !TypeParam::bit_size), "Unsupported size type");
	}

	EXPECT_EQ(a0-b0, c0);
	EXPECT_EQ(a1-b1, c1);
	EXPECT_EQ(a2-b2, c2);
}
