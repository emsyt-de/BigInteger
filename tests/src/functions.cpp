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

TYPED_TEST(BigIntegerTests, CheckMSB)
{
	if constexpr(TypeParam::bit_size == 128)
	{
		using namespace uint128_ct;

		EXPECT_EQ(TypeParam::bits(0x746789abc34_num), 42);
		EXPECT_EQ(TypeParam::bits(0xf789abc346789ab8e0e81dbb63e5be57_num), 127);
		if constexpr(proof_const)
		{
			/// Test compile time computing
			static_assert(TypeParam::bits(0x746789abc34_num) == 42, "MSB function failed");
			static_assert(TypeParam::bits(0xf789abc346789ab8e0e81dbb63e5be57_num) == 127, "MSB function failed");
		}
	}
	else if constexpr(TypeParam::bit_size == 256)
	{
		using namespace uint256_t;

		EXPECT_EQ(TypeParam::bits(0xf46789abc346789abc34678234769ab4654c34678932dabc3467_num), 207);
		EXPECT_EQ(TypeParam::bits(0xf789abc346789ab8e0e81dbb63e5be577e2a2b1b_num), 159);
		if constexpr(proof_const)
		{
			/// Test compile time computing
			static_assert(TypeParam::bits(0xf46789abc346789abc34678234769ab4654c34678932dabc3467_num) == 207, "MSB function failed");
			static_assert(TypeParam::bits(0xf789abc346789ab8e0e81dbb63e5be577e2a2b1b_num) == 159, "MSB function failed");
		}
	}
	else if constexpr(TypeParam::bit_size == 512)
	{
		using namespace uint512_t;

		EXPECT_EQ(TypeParam::bits(0xf46789abc346789abc34678234769ab4654c34678932dabc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346_num), 479);
		EXPECT_EQ(TypeParam::bits(0xf789abc346789ab8e0e81dbb63e5be577e2a2b1bc789ab8edeeeef289abc346789abc346789abc346789_num), 335);
		if constexpr(proof_const)
		{
			/// Test compile time computing
			static_assert(TypeParam::bits(0xf46789abc346789abc34678234769ab4654c34678932dabc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346_num) == 479, "MSB function failed");
			static_assert(TypeParam::bits(0xf789abc346789ab8e0e81dbb63e5be577e2a2b1bc789ab8edeeeef289abc346789abc346789abc346789_num) == 335, "MSB function failed");
		}
	}
	else if constexpr(TypeParam::bit_size == 1024)
	{
		using namespace uint1024_t;

		EXPECT_EQ(TypeParam::bits(0xf46789abc346789abc34678234769ab4654c34678932dabc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346_num), 479);
		EXPECT_EQ(TypeParam::bits(0xf789abc346789ab8e0e81dbb63e5be577e2a2b1bc789ab8edeeeef289abc346789abc346789abc346789_num), 335);
		if constexpr(proof_const)
		{
			/// Test compile time computing
			static_assert(TypeParam::bits(0xf46789abc346789abc34678234769ab4654c34678932dabc346789abc3467bc346789abc346789abc346789abc346789abc346789abc346789abc346_num) == 479, "MSB function failed");
			static_assert(TypeParam::bits(0xf789abc346789ab8e0e81dbb63e5be577e2a2b1bc789ab8edeeeef289abc346789abc346789abc346789_num) == 335, "MSB function failed");
		}
	}
	else
	{
		/// Expression is always false but depends on bit_size
		static_assert (!(TypeParam::bit_size || !TypeParam::bit_size), "Unsupported size type");
	}
}
