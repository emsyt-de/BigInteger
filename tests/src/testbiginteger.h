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

#include "gtest/gtest.h"

#include "biginteger.h"

using namespace biginteger;
using ::testing::Types;

template <typename T>
class BigIntegerTests : public ::testing::Test {
};

typedef Types<uint128_ct::uint128_ct, uint256_t::uint256_t, uint512_t::uint512_t, uint1024_t::uint1024_t> BigIntegerTypes;

TYPED_TEST_SUITE(BigIntegerTests, BigIntegerTypes);
