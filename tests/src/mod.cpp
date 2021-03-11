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

TYPED_TEST(BigIntegerTests, Modulo)
{
	TypeParam  a0,a1;
	TypeParam  b0,b1;
	TypeParam  c1;
	if constexpr(std::is_same_v<TypeParam,uint128_t>)
	{
		using namespace uint128;
		a0 = 0xff4b9c63cbd74d45_num;
		a1 = 0xff4b9c63cbd74d45_num;
		b0 = 0x0_num;
		b1 = 0xff45d391f11421ae_num;
		c1 = 0x5c8d1dac32b97_num;

		if constexpr(proof_const)
		{
			/// Test compile time computing
			constexpr TypeParam ab3 = 0xff4b9c63cbd74d45_num % 0xff45d391f11421ae_num;
			constexpr TypeParam c3  = 0x5c8d1dac32b97_num;
			static_assert (ab3 == c3, "Modulo failed");
		}
	}
	else if constexpr(std::is_same_v<TypeParam,uint256_t>)
	{
		using namespace uint256;
		a0 = 0x17fff_num;
		a1 = 0x80000000000000000000000000000_num;
		b0 = 0x0_num;
		b1 = 0x20000000000000000000000000_num;
		c1 = 0x0_num;

		if constexpr(proof_const)
		{
			/// Test compile time computing
			constexpr TypeParam ab3 = 0x8000000000000000_num % 0x80000000_num;
			constexpr TypeParam c3  = 0x0_num;
			static_assert (ab3 == c3, "Modulo failed");
		}
	}
	else if constexpr(std::is_same_v<TypeParam,uint512_t>)
	{
		using namespace uint512;
		a0 = 0xf_num;
		a1 = 0x1ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffb0_num;
		b0 = 0x0_num;
		b1 = 0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffed_num; // 2^255-19
		c1 = 0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe9_num;

		if constexpr(proof_const)
		{
			/// Test compile time computing
			constexpr TypeParam ab3 = 0xf46789abc346789abc34678234769ab4654c34678932dabc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346_num
					% 0xf789abc346789ab8e0e81dbb63e5be577e2a2b1bc789ab8edeeeef289abc346789abc346789abc346789_num;
			constexpr TypeParam c3  = 0x2ad9dd29f03a4cefa131ea7c36052af84f4f6622eb12dedbd433560237ce1670901b91f59c9970c0486a_num;
			static_assert (ab3 == c3, "Modulo failed");
		}
	}
	else if constexpr(std::is_same_v<TypeParam,int512_t>)
	{
		using namespace int512;
		a0 = 0xf_num;
		a1 = 0x1ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffb0_num;
		b0 = 0x0_num;
		b1 = 0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffed_num; // 2^255-19
		c1 = 0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe9_num;

		if constexpr(proof_const)
		{
			/// Test compile time computing
			constexpr TypeParam ab3 = 0xf46789abc346789abc34678234769ab4654c34678932dabc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346_num
					% 0xf789abc346789ab8e0e81dbb63e5be577e2a2b1bc789ab8edeeeef289abc346789abc346789abc346789_num;
			constexpr TypeParam c3  = 0x2ad9dd29f03a4cefa131ea7c36052af84f4f6622eb12dedbd433560237ce1670901b91f59c9970c0486a_num;
			static_assert (ab3 == c3, "Modulo failed");
		}
	}
	else if constexpr(std::is_same_v<TypeParam,uint1024_t>)
	{
		using namespace uint1024;
		a0 = 0xf_num;
		a1 = 0x18b035581830722386091c50d922d0779b4364b931ba579aa3cd50aadc768500498d1bc95f9ffb2d91685cdf47c1c7a705cd635cacae5ea65b7789ed126b8a8413c5d7025e4fd791011bbf02e68936c9c131c835c338b254a00dcd5a42df364a3cffba6ebfd2066d6b13e5ad895850cc41f01633dab4aa76_num;
		b0 = 0x0_num;
		b1 = 0x346789ab8edeeeeef8edeeeeef8edeeeeef8edeeeeef8edef21783321ef8edeeeeef8edeeeeef8edeeeeef8edeeeeef8edeeeeef8edeeeeef8b9876544_num;
		c1 = 0x27f002c35eec80436073fc9eedeff4e28cbacb3334a784c4a94504ec2358cf3ca80f5d91c76ba24c9efcbc02eeb570634178cb204f1c4370d44c2593fe_num;

		if constexpr(proof_const)
		{
			/// Test compile time computing
			constexpr TypeParam ab3 = 0x18b035581830722386091c50d922d0779b4364b931ba579aa3cd50aadc768500498d1bc95f9ffb2d91685cdf47c1c7a705cd635cacae5ea65b7789ed126b8a8413c5d7025e4fd791011bbf02e68936c9c131c835c338b254a00dcd5a42df364a3cffba6ebfd2066d6b13e5ad895850cc41f01633dab4aa76_num
					% 0x346789ab8edeeeeef8edeeeeef8edeeeeef8edeeeeef8edef21783321ef8edeeeeef8edeeeeef8edeeeeef8edeeeeef8edeeeeef8edeeeeef8b9876544_num;
			constexpr TypeParam c3  = 0x27f002c35eec80436073fc9eedeff4e28cbacb3334a784c4a94504ec2358cf3ca80f5d91c76ba24c9efcbc02eeb570634178cb204f1c4370d44c2593fe_num;
			static_assert (ab3 == c3, "Modulo failed");
		}
	}
	else
	{
		/// Expression is always false but depends on bit_size
		static_assert (!(TypeParam::bit_size || !TypeParam::bit_size), "Unsupported size type");
	}

	EXPECT_THROW(a0 % b0,std::invalid_argument);
	EXPECT_EQ(a1 % b1, c1);
}
