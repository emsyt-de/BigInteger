/*
 * This file is part of the BigInteger distribution (https://github.com/xxxx).
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

#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <concepts>
#include <utility>
#include <array>
#include <iostream>

typedef __uint128_t uint128_t;
typedef __int128_t int128_t;

namespace biginteger {

template <class, template <typename,std::size_t ...> class>
struct is_instance : public std::false_type {};

template <typename B, size_t... T, template <typename,std::size_t ...> class U>
struct is_instance<U<B,T...>, U> : public std::true_type {};

/// define extended integral concept
template <typename T1>
concept ext_integral = std::is_integral_v<T1> || requires(T1 t1)
{
{ t1 };
{ T1::numbers };
//{ t1 + t1 } -> std::same_as<T1>;
//{ t1 - t1 } -> std::same_as<T1>;
//{ t1 * t1 } -> std::same_as<T1>;
//{ t1 / t1 } -> std::same_as<T1>;
};


/// base class
template<typename B, std::size_t ...I>
class BigInteger
{

public:

	/// Constructors
	constexpr BigInteger() = default;
	constexpr BigInteger(const BigInteger & n) = delete;
	constexpr BigInteger(BigInteger && n) = default;

	template<std::integral ...T, std::enable_if_t<sizeof... (T) <= sizeof... (I),int> = 0>
	constexpr BigInteger(const T & ... n)
		: numbers{static_cast<typename decltype(numbers)::value_type>(n)...}
	{
	}

	/// Assignment Operators
	BigInteger & operator=(const BigInteger & n) = delete;
	BigInteger & operator=(BigInteger && n) = default;

	/// Typecast Operators
//	constexpr operator bool() const
//	{
//		return (numbers[I] || ...);
//	}

	/// Bitwise Operators

	/// And
	template <ext_integral T1, ext_integral T2>
	friend inline constexpr BigInteger operator&(const T1 & l, const T2 & r)
	{
		if constexpr (is_instance<T1,BigInteger>{} && is_instance<T2,BigInteger>{})
		{
			return ((l.numbers[I] & r.numbers[I]) ,...);
		}
		else if constexpr (std::is_integral_v<T2>)
		{
			return l & BigInteger(r);
		}
		else if constexpr (std::is_integral_v<T1>)
		{
			return BigInteger(l) & r;
		}
		else
		{
			return r & l;
		}
	}

	template <ext_integral T1, ext_integral T2>
	friend inline constexpr BigInteger operator&=(T1 & l, const T2 & r)
	{
		return l = (l & r);
	}

	/// Or
	template <ext_integral T1, ext_integral T2>
	friend inline constexpr BigInteger operator|(const T1 & l, const T2 & r)
	{
		if constexpr (is_instance<T1,BigInteger>{} && is_instance<T2,BigInteger>{})
		{
			return ((l.numbers[I] | r.numbers[I]), ...);
		}
		else if constexpr (std::is_integral_v<T2>)
		{
			return l | BigInteger(r);
		}
		else if constexpr (std::is_integral_v<T1>)
		{
			return BigInteger(l) | r;
		}
		else
		{
			return r | l;
		}
	}

	template <ext_integral T1, ext_integral T2>
	friend inline constexpr BigInteger operator|=(T1 & l, const T2 & r)
	{
		return l = (l | r);
	}

	/// Xor
	template <ext_integral T1, ext_integral T2>
	friend inline constexpr BigInteger operator^(const T1 & l, const T2 & r)
	{
		if constexpr (is_instance<T1,BigInteger>{} && is_instance<T2,BigInteger>{})
		{
			return ((l.numbers[I] ^ r.numbers[I]), ...);
		}
		else if constexpr (std::is_integral_v<T2>)
		{
			return l ^ BigInteger(r);
		}
		else if constexpr (std::is_integral_v<T1>)
		{
			return BigInteger(l) ^ r;
		}
		else
		{
			return r ^ l;
		}
	}

	template <ext_integral T1, ext_integral T2>
	friend inline constexpr BigInteger operator^=(T1 & l, const T2 & r)
	{
		return l = (l ^ r);
	}

	/// Invert operator
	inline BigInteger operator~() const {
		return (~numbers[I] , ...);
	}

	/// Comparison Operators

	/// Equal
	friend inline constexpr bool operator==(const BigInteger & l, const BigInteger & r)
	{
		return ((l.numbers[I] == r.numbers[I]) && ...);
	}

	/// Three way compare
	friend inline constexpr std::strong_ordering operator<=>(const BigInteger & l, const BigInteger & r)
	{
		auto ret = std::strong_ordering::equal;
		((ret = (std::strong_ordering::equal != l.numbers[I] <=> r.numbers[I]) ? l.numbers[I] <=> r.numbers[I] : ret), ...);
		return ret;
	}

	/// Arithmetic Operators

	/// Plus
	template <ext_integral T1, ext_integral T2>
	friend inline constexpr BigInteger operator+(const T1 & l, const T2 & r)
	{
		if constexpr (is_instance<T1,BigInteger>{} && is_instance<T2,BigInteger>{})
		{
			BigInteger sum;
			((std::get<I>(sum.numbers) = std::get<I>(l.numbers) + std::get<I>(r.numbers)), ...);
			((std::get<I>(sum.numbers) += (I > 0 && sum.numbers[I-1] < l.numbers[I-1]) ? 1 : 0), ...);
			return std::move(sum);
		}
		else if constexpr (std::is_integral_v<T1>)
		{
			return static_cast<BigInteger>(l) + r;
		}
		else if constexpr (std::is_integral_v<T2>)
		{
			return l + static_cast<BigInteger>(r);
		}
		else
		{
			static_assert (!(std::is_integral_v<T1> || std::is_integral_v<T2>), "Wrong plus operator overload");
			return r + l;
		}
	}

	template <ext_integral T2>
	friend inline constexpr BigInteger & operator+=(BigInteger & l, const T2 & r){
		return l = l + r;
	}

	/// Minus
	template <ext_integral T1, ext_integral T2>
	friend constexpr inline BigInteger operator-(const T1 & l, const T2 & r)
	{
		if constexpr (is_instance<T1,BigInteger>{} && is_instance<T2,BigInteger>{})
		{
			BigInteger diff;
			((std::get<I>(diff.numbers) = std::get<I>(l.numbers) - std::get<I>(r.numbers)), ...);
			((std::get<I>(diff.numbers) -= (I > 0 && diff.numbers[I-1] > l.numbers[I-1]) ? 1 : 0), ...);
			return diff;
		}
		else if constexpr (std::is_integral_v<T1>)
		{
			return static_cast<BigInteger>(l) - r;
		}
		else if constexpr (std::is_integral_v<T2>)
		{
			return l - static_cast<BigInteger>(r);
		}
		else
		{
			static_assert (!(std::is_integral_v<T1> || std::is_integral_v<T2>), "Wrong minus operator overload");
			return r - l;
		}
	}


	template <ext_integral T2>
	friend inline constexpr BigInteger & operator-=(BigInteger & l, const T2 & r){
		return l = l - r;
	}

	/// Multiply
	template <ext_integral T1, ext_integral T2>
	friend inline constexpr BigInteger operator*(const T1 & l, const T2 & r){
		if constexpr (is_instance<T1,BigInteger>{} && is_instance<T2,BigInteger>{})
		{
			BigInteger diff;
			return std::move(diff);
		}
		else if constexpr (std::is_integral_v<T1>)
		{
			return static_cast<BigInteger>(l) * r;
		}
		else if constexpr (std::is_integral_v<T2>)
		{
			return l * static_cast<BigInteger>(r);
		}
		else
		{
			static_assert (!(std::is_integral_v<T1> || std::is_integral_v<T2>), "Wrong multiply operator overload");
			return r * l;
		}
	}

	template <ext_integral T2 >
	friend inline constexpr BigInteger & operator*=(BigInteger & l, const T2 & r){
		return l = l * r;
	}

	template<char ...digits>
	friend inline consteval BigInteger operator "" _num() noexcept
	{
		constexpr std::array<char, sizeof...(digits)> digits_array{ digits... };
		// TODO fix max numbers
		constexpr bool is_hex = sizeof... (digits) > 2 && sizeof... (digits) < 60 && digits_array[0] == '0' && digits_array[1] == 'x';
		constexpr bool is_oct = sizeof... (digits) > 1 && sizeof... (digits) < 93 && digits_array[0] == '0' &&
				(('0' <= digits <= '7') && ...);
		constexpr bool is_dec = sizeof... (digits) > 0 && sizeof... (digits) < 70;
		static_assert(is_hex || is_oct || is_dec, "invalid digit in number");
		if constexpr(is_hex)
		{
			// remove hex 0x
			// extract in batches with max 16^n hex number for 64-bit (2^64 == 16^16)
			return stoi<digits_array.size()-2>(digits_array.data()+2,16);
		}
		else if constexpr(is_oct)
		{
			// remove oct 0
			// extract in batches with max 8^n dec number for 64-bit (2^63 == 8^21)
			return stoi<digits_array.size()-1>(digits_array.data()+1,8);
		}
		else if constexpr(is_dec)
		{
			// extract in batches with max 10^n dec number for 64-bit (10^19)
			return stoi<digits_array.size()>(digits_array.data(),10);
		}
	}

	/// IO Operators
	friend std::ostream & operator<<(std::ostream & stream, const BigInteger & r)
	{
		return (stream<<"[ ",((stream << std::hex << static_cast<uint64_t>(r.numbers[I]) << (I < r.numbers.size()-1 ? ", ":" ")), ...),stream<<"]");
	}

	/// Get order of msb bit.
	/// Return [0 ... ((sizeof(block) * #block * 8) - 1)].
	consteval auto bits() const{
		constexpr auto bits_per_block = sizeof (B) * 8;
		int16_t out = bits_per_block * numbers.size() - 1;
		static_assert (sizeof (B) > sizeof (uint64_t), "Unsupported block size");
		auto it = numbers.rbegin();
		while (it != numbers.rend())
		{
			if(*it > 0) {
				out -= static_cast<int16_t>(__builtin_clzll(*it));
				break;
			}
			else
				out -= bits_per_block;
		}
		if(out < 0)
			out = 0;
		return out;
	}

private:
	template<std::size_t size>
	static inline consteval void stoi_impl(const char* str, const BigInteger & base, BigInteger & value)
	{
		auto c = str[0];
		auto i = c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c - 'A' + 10;
		if constexpr(size > 1)
		{
			value = value * base + i;
			stoi_impl<size-1>(str + 1, base, value);
		}
	}

	template<std::size_t size>
	static inline consteval BigInteger stoi(const char* str, const BigInteger & base)
	{
		BigInteger result = 0;
		stoi_impl<size>(str, base,result);
		return result;
	}

	/// {} makes value initialization for non class non array types like int to its default value 0
	std::array<B,sizeof... (I)> numbers{};
};

typedef BigInteger<uint64_t,0,1,2,3,4,5,6,7> uint512_t;
typedef BigInteger<uint64_t,0,1,2,3> uint256_t;

/// literal operators
template<char ...digits>
inline consteval uint512_t operator "" _num() noexcept;
//template<char ...digits>
//inline consteval uint255_t operator "" _num() noexcept;


}

#endif // BIGINTEGER_H
