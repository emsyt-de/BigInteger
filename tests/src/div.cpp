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

static constexpr bool proof_const = true;

TYPED_TEST(BigIntegerTests, Divide)
{
	TypeParam  a0,a1;
	TypeParam  b0,b1;
	TypeParam  c1;
	if constexpr(TypeParam::bit_size == 128)
	{
		using namespace uint128_ct;
		a0 = 0xf_num;
		a1 = 0xffffffff322123ff_num;
		b0 = 0x0_num;
		b1 = 0xffffffff_num;
		c1 = 0x100000000_num;

		if constexpr(proof_const)
		{
			/// Test compile time computing
			constexpr TypeParam ab3 = 0xff4b9c63cbd74d45_num / 0xff45d391f11421ae_num;
			constexpr TypeParam c3  = 0x1u;
			static_assert (ab3 == c3, "Multiply failed");
		}
	}
	else if constexpr(TypeParam::bit_size == 256)
	{
		using namespace uint256_t;
		a0 = 0xf_num;
		a1 = 0xffffffff_num;
		b0 = 0x0_num;
		b1 = 0xffffffff_num;
		c1 = 0x1_num;

		if constexpr(proof_const)
		{
			/// Test compile time computing
			constexpr TypeParam ab3 = 0xfffffffff_num/0xfffffffff_num;
			constexpr TypeParam c3  = 0x1_num;
			static_assert (ab3 == c3, "Divide failed");
		}
	}
	else if constexpr(TypeParam::bit_size == 512)
	{
		using namespace uint512_t;
		a0 = 0xf_num;
		a1 = 0xf46789abc346789abc34678234769ab4654c34678932dabc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346_num;
		b0 = 0x0_num;
		b1 = 0xf789abc346789ab8e0e81dbb63e5be577e2a2b1bc789ab8edeeeef289abc346789abc346789abc346789_num;
		c1 = 0xfcc27208580b0ffd63986f717697c3b510fc_num;

		if constexpr(proof_const)
		{
			/// Test compile time computing
			constexpr TypeParam ab3 = 0x1c17726544a6d8d83608f44a918f2c2e136fe05177dc6cde00e81dbb63e5be587eacae577e2a2b1bc096ac5ee51bdc2f48e2e4d98a3740bf020_num
					/ 0x893ace7477dca3262672246789abc346789abc346789abc346789abc_num;
			constexpr TypeParam c3  = 0x346789abc346789abc346789abc346789abc346789abc346789abc34678_num;
			static_assert (ab3 == c3, "Divide failed");
		}
	}
	else if constexpr(TypeParam::bit_size == 1024)
	{
		using namespace uint1024_t;
		a0 = 0xf_num;
		a1 = 0x18b035581830722386091c50d922d0779b4364b931ba579aa3cd50aadc768500498d1bc95f9ffb2d91685cdf47c1c7a705cd635cacae5ea65b7789ed126b8a8413c5d7025e4fd791011bbf02e68936c9c131c835c338b254a00dcd5a42df364a3cffba6ebfd2066d6b13e5ad895850cc41f01633dab4aa76_num;
		b0 = 0x0_num;
		b1 = 0x346789ab8edeeeeef8edeeeeef8edeeeeef8edeeeeef8edef21783321ef8edeeeeef8edeeeeef8edeeeeef8edeeeeef8edeeeeef8edeeeeef8b9876544_num;
		c1 = 0x789abc34e02467c2eea64367fe501bf8151491204f5c36524b0bca462cfc8340c3f497b2da0c9446e59acdcbc7a0d794620e5b96b810c812d5e43e_num;

		if constexpr(proof_const)
		{
			/// Test compile time computing
			constexpr TypeParam ab3 = 0x346789abc346789abc34678234769ab4654c34678932dabc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346_num
					/ 0x789abc346789ab8e0e81dbb63e5be577e2a2b1bc789ab8edeeeef289abc346789abc346789abc346789_num;
			constexpr TypeParam c3  = 0x6f3c6137d34bbf0e51a764ba78160cce89729_num;
			static_assert (ab3 == c3, "Divide failed");
		}
	}
	else
	{
		/// Expression is always false but depends on bit_size
		static_assert (!(TypeParam::bit_size || !TypeParam::bit_size), "Unsupported size type");
	}

	EXPECT_THROW(a0/b0,std::invalid_argument);
	EXPECT_EQ(a1/b1, c1);
}
