/*
 * This file is part of the XXX distribution (https://github.com/xxxx).
 * Copyright (c) 2020 Martin Schuler.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>

#include "testbiginteger.h"

static constexpr bool proof_const = true;

TYPED_TEST(BigIntegerTests, Invert)
{
	constexpr TypeParam a = TypeParam::max();
	constexpr TypeParam b = TypeParam::min();

	if constexpr (proof_const)
	{
		static_assert (a == ~b, "Invert failed");
		static_assert (~a == b, "Invert failed");
	}

	EXPECT_EQ( a, ~b);
	EXPECT_EQ(~a,  b);
}
