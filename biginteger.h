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
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <concepts>
#include <utility>
#include <array>
#include <functional>
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

inline std::ostream & operator<<(std::ostream & stream, const uint128_t & r)
{
	uint64_t l = static_cast<uint64_t>(r);
	uint64_t u = r>>64;
	if(u > 0)
		return stream<<std::showbase<<u<<std::noshowbase<<std::setfill('0')<<std::setw(16)<<l<<std::showbase;
	else
		return stream<<std::showbase<<l;

}

/// base class
template<std::unsigned_integral B, std::size_t ...I>
class BigInteger
{

public:

	/// Constructors
	constexpr BigInteger() = default;
	constexpr BigInteger(const BigInteger & n) = delete;
	constexpr BigInteger(BigInteger && n) = default;

	template<std::unsigned_integral ...T, std::enable_if_t<sizeof... (T) <= sizeof... (I),int> = 0>
	constexpr BigInteger(const T & ... n)
		: numbers{n...}
	{}

public:
	/// Assignment Operators
	BigInteger & operator=(const BigInteger & n) = delete;
	constexpr BigInteger & operator=(BigInteger && n) = default;

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
			return BigInteger((l.numbers[I] & r.numbers[I]) ...);
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
	friend inline constexpr BigInteger& operator&=(T1 & l, const T2 & r)
	{
		return l = (l & r);
	}

	/// Or
	template <ext_integral T1, ext_integral T2>
	friend inline constexpr BigInteger operator|(const T1 & l, const T2 & r)
	{
		if constexpr (is_instance<T1,BigInteger>{} && is_instance<T2,BigInteger>{})
		{
			return BigInteger((l.numbers[I] | r.numbers[I]) ...);
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
	friend inline constexpr BigInteger& operator|=(T1 & l, const T2 & r)
	{
		return l = (l | r);
	}

	/// Xor
	template <ext_integral T1, ext_integral T2>
	friend inline constexpr BigInteger operator^(const T1 & l, const T2 & r)
	{
		if constexpr (is_instance<T1,BigInteger>{} && is_instance<T2,BigInteger>{})
		{
			return BigInteger((l.numbers[I] ^ r.numbers[I]) ...);
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
	friend inline constexpr BigInteger& operator^=(T1 & l, const T2 & r)
	{
		return l = (l ^ r);
	}

	/// Invert operator
	inline BigInteger operator~() const
	{
		return {std::forward<B>(~numbers[I])...};
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
			return sum;
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
			BigInteger result;
			std::array<B,sizeof... (I)*2> temp_l,temp_r;
			std::array<std::array<B,sizeof... (I)*2>,sizeof... (I)*2> product;
			constexpr auto num_bits = std::numeric_limits<B>::digits;
			constexpr auto num_bits_half = num_bits >> 1;
			constexpr B mask = std::numeric_limits<B>::max()>>num_bits_half;
			for(std::size_t i = 0; i < sizeof... (I); i++)
			{
				std::size_t ii = i<<1;
				temp_l[ii]   = l.numbers[i] & mask;
				temp_l[ii+1] = l.numbers[i]>>num_bits_half;
				temp_r[ii]   = r.numbers[i] & mask;
				temp_r[ii+1] = r.numbers[i]>>num_bits_half;
			}
			for(std::size_t i = 0; i < temp_l.size(); i++)
			{
				for(std::size_t j = 0; j < temp_r.size(); j++)
				{
					product[i][j] = temp_l[i]*temp_r[j];
				}
			}
			for(std::size_t jj = 0; jj < temp_l.size(); jj++)
			{
				for(std::size_t ii = 0; ii + jj < temp_r.size(); ii++)
				{
					auto j = jj>>1;
					auto i = ii>>1;
					auto temp0 = result.numbers[i+j];
					decltype (temp0) temp1 = 0;
					if((i+j) < result.numbers.size() - 1)
					{
						temp1 = result.numbers[i+j+1];
					}

					if(jj & 1 && ii & 1 && (i + j) < result.numbers.size() - 1)
					{
						result.numbers[i+j+1] += product[ii][jj];
					}
					else if((jj & 1 || ii & 1) && (i + j) < result.numbers.size() - 1)
					{
						result.numbers[i+j] += product[ii][jj] << num_bits_half;
						result.numbers[i+j+1] += product[ii][jj] >> num_bits_half;
					}
					else if((jj & 1 || ii & 1) && (i + j) < result.numbers.size() )
					{
						result.numbers[i+j] += (product[ii][jj] & mask) << num_bits_half;
					}
					else
					{
						result.numbers[i+j] += product[ii][jj];
					}

					if((i+j) < result.numbers.size() - 2 && temp1 > result.numbers[i+j+1])
					{
						result.numbers[i+j+2] += 1;
					}
					if((i+j) < result.numbers.size() - 1 && temp0 > result.numbers[i+j])
					{
						temp1 = result.numbers[i+j+1];
						result.numbers[i+j+1] += 1;
						if((i+j) < result.numbers.size() - 2 && temp1 > result.numbers[i+j+1])
						{
							result.numbers[i+j+2] += 1;
						}
					}
				}
			}
			return result;
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

	/// Divide
	template <ext_integral T1, ext_integral T2>
	friend inline constexpr BigInteger operator/(const T1 & l, const T2 & r){
		if constexpr (is_instance<T1,BigInteger>{} && is_instance<T2,BigInteger>{})
		{
			return divmod(l,r).first;
		}
		else if constexpr (std::is_integral_v<T1>)
		{
			return static_cast<BigInteger>(l) / r;
		}
		else if constexpr (std::is_integral_v<T2>)
		{
			return l / static_cast<BigInteger>(r);
		}
		else
		{
			static_assert (!(std::is_integral_v<T1> || std::is_integral_v<T2>), "Wrong divide operator overload");
			return r / l;
		}
	}

	template <ext_integral T2 >
	friend inline constexpr BigInteger & operator/=(BigInteger & l, const T2 & r){
		return l = l / r;
	}

	/// Bit Shift Operators

	/// Shift left
	template <ext_integral T1, std::unsigned_integral T2>
	friend inline constexpr BigInteger operator<<(const T1 & l, const T2 & r)
	{
		if constexpr (std::is_same_v<T1,BigInteger>)
		{
			BigInteger result;
			std::size_t msb = (sizeof(sizeof (B))<<3) - __builtin_clzl(sizeof (B)) - 1;
			std::size_t chunk_shift = r>>(3+msb);
			std::size_t left = r - ((chunk_shift*sizeof (B))<<3);
			for(std::size_t i = chunk_shift; i < result.numbers.size(); i++)
			{
				result.numbers[i] = l.numbers[i-chunk_shift];
			}
			if(left) // 0...sizeof(B)*8 bit shift is left
			{
				result.numbers[chunk_shift] <<= left;
				std::size_t right_shift = (sizeof (B)<<3)-left;
				for(std::size_t i = chunk_shift + 1; i < l.numbers.size(); i++)
				{
					result.numbers[i] <<= left;
					result.numbers[i] |= l.numbers[i-chunk_shift-1] >> right_shift;
				}
			}
			return result;
		}
		else if constexpr (std::is_integral_v<T1>)
		{
			return static_cast<BigInteger>(l) << r;
		}
		else
		{
			static_assert (!(std::is_integral_v<T1>), "Wrong shift left operator overload");
			return r << l;
		}
	}

	template <std::unsigned_integral T2>
	friend inline constexpr BigInteger& operator<<=(BigInteger & l, const T2 & r)
	{
		return l = l << r;
	}

	/// Shift right
	template <ext_integral T1, std::unsigned_integral T2>
	friend inline constexpr BigInteger operator>>(const T1 & l, const T2 & r)
	{
		if constexpr (std::is_same_v<T1,BigInteger>)
		{
			BigInteger result;
			std::size_t msb = (sizeof(sizeof (B))<<3) - __builtin_clzl(sizeof (B)) - 1;
			std::size_t chunk_shift = r>>(3+msb);
			std::size_t left = r - ((chunk_shift*sizeof (B))<<3);
			for(std::size_t i = chunk_shift; i < result.numbers.size(); i++)
			{
				result.numbers[i-chunk_shift] = l.numbers[i];
			}
			if(left) // 0...sizeof(B)*8 bit shift is left
			{
				result.numbers[l.numbers.size() - chunk_shift - 1] >>= left;
				std::size_t left_shift = (sizeof (B)<<3)-left;
				for(std::size_t i = 0; i < l.numbers.size() - chunk_shift - 1; i++)
				{
					result.numbers[i] >>= left;
					result.numbers[i] |= l.numbers[i + chunk_shift + 1] << left_shift;
				}
			}
			return result;
		}
		else if constexpr (std::is_integral_v<T1>)
		{
			return static_cast<BigInteger>(l) << r;
		}
		else
		{
			static_assert (!(std::is_integral_v<T1>), "Wrong shift left operator overload");
			return r << l;
		}
	}

	template <std::unsigned_integral T2>
	friend inline constexpr BigInteger& operator>>=(BigInteger & l, const T2 & r)
	{
		return l = l >> r;
	}

	/// IO Operators
	friend std::ostream & operator<<(std::ostream & stream, const BigInteger & r)
	{
		return (stream<<"[ ",((stream << std::hex << (r.numbers[I]) << (I < r.numbers.size()-1 ? ", ":" ")), ...),stream<<"]");
	}

	/// Get order of msb bit.
	/// Return [0 ... ((sizeof(block) * #block * 8) - 1)].
	static constexpr auto bits(const BigInteger& n) {
		constexpr auto bits_per_block = sizeof (B) * 8;
		std::size_t out = bits_per_block * n.numbers.size();
		auto it = n.numbers.rbegin();
		while (it != n.numbers.rend())
		{
			if(*it > 0)
			{
				if constexpr(sizeof (B) == 2 * sizeof (uint64_t))
				{
					if(B n = *it >> (sizeof (uint64_t)<<3); n > 0)
					{
						out -= static_cast<std::size_t>(__builtin_clzll(n));
					}
					else
					{
						out -= 64;
						out -= static_cast<std::size_t>(__builtin_clzll(*it));
					}
				}
				else if constexpr(sizeof (B) <= sizeof (uint64_t))
				{
					out -= static_cast<std::size_t>(__builtin_clzll(*it));
				}
				else
				{
					static_assert(sizeof (B) == 0, "Unsupported type");
				}
				break;
			}
			else
			{
				out -= bits_per_block;
			}
			it++;
		}
		return out > 0 ? out - 1 : 0;
	}

	static constexpr std::size_t bit_size = sizeof(B) * sizeof... (I) * 8;
	static consteval BigInteger max() noexcept
	{
		BigInteger n;
		((n.numbers[I] = std::numeric_limits<B>::max()), ...);
		return n;
	}

	template<std::unsigned_integral T>
	static constexpr BigInteger exp(BigInteger&& x, T n)
	{
		return exp_by_squaring(1u, std::forward<BigInteger>(x), n);
	}

private:
	template<std::unsigned_integral T>
	static constexpr BigInteger exp_by_squaring(BigInteger&& y, BigInteger&& x, T n)
	{
		if(n == 0)
			return std::forward<BigInteger>(y);
		else if(n == 1)
			return x * y;
		else if(n & 1) // odd
			return exp_by_squaring( x * y, x * x, (n - 1)>>1);
		else // even
			return exp_by_squaring( std::forward<BigInteger>(y), x * x, n>>1);
	}

	template<char ...digits>
	static constexpr BigInteger to_number() noexcept
	{
		constexpr std::array<char, sizeof...(digits)> digits_array{ digits... };
		constexpr bool is_hex = sizeof... (digits) > 2 && sizeof... (digits) <= ((bit_size>>2)+2) && digits_array[0] == '0' && digits_array[1] == 'x';
		constexpr bool is_oct = sizeof... (digits) > 1 && sizeof... (digits) <= (bit_size/3+1) && digits_array[0] == '0' &&
				(('0' <= digits && digits <= '7') && ...);
		constexpr bool is_dec = sizeof... (digits) > 0 && sizeof... (digits) <= (static_cast<float>(bit_size)/3.32f) &&
				(('0' <= digits && digits <= '9') && ...);
		static_assert(is_hex || is_oct || is_dec, "invalid digit in number");
		if constexpr(is_hex)
		{
			// remove hex 0x
			return stoi<2>(std::forward<decltype (digits_array)>(digits_array),16u);
		}
		else if constexpr(is_oct)
		{
			// remove oct 0
			return stoi<1>(std::forward<decltype (digits_array)>(digits_array),8u);
		}
		else if constexpr(is_dec)
		{
			return stoi<0>(std::forward<decltype (digits_array)>(digits_array),10u);
		}
	}

	template<std::size_t index, std::size_t size>
	static constexpr BigInteger stoi_impl(const std::array<char,size>&& str, BigInteger&& base, BigInteger&& value)
	{
		auto c = str[index];
		B i = c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c - 'A' + 10;
		if constexpr(index < size - 1)
		{
			return stoi_impl<index+1>(std::forward<decltype (str)>(str), std::forward<BigInteger>(base), value * base + i);
		}
		else
		{
			return value * base + i;
		}
	}

	template<std::size_t index, std::size_t size>
	static constexpr BigInteger stoi(const std::array<char,size>&& str, BigInteger && base)
	{
		return stoi_impl<index>(std::forward<decltype (str)>(str), std::forward<BigInteger>(base), 0u);
	}

	static constexpr std::pair<BigInteger,BigInteger> divmod(const BigInteger & l, const BigInteger & r)
	{
		auto msb_l = bits(l);
		auto msb_r = bits(r);
		BigInteger one(1u);
		BigInteger zero(0u);
		if(r == zero)
		{
			throw std::invalid_argument("Devide with zero!");
		}
		else if(l < r)
		{
			return std::make_pair(BigInteger(0u),l<<0u);  // use noop to copy l to result
		}
		BigInteger ll = l << 0u; // use noop to copy const value l to ll
		BigInteger rr = r << (msb_l-msb_r);
		BigInteger i = one << (msb_l-msb_r);
		BigInteger div,mod;
		while(i > zero)
		{
			if(ll >= rr)
			{
				div |= i;
				ll -= rr;
			}
			rr >>= 1u;
			i >>= 1u;
		}
		return std::make_pair(std::forward<BigInteger>(div),std::forward<BigInteger>(mod));
	}

	/// {} makes value initialization for non class non array types like int to its default value 0
	std::array<B,sizeof... (I)> numbers{};
};


namespace uint128_ct {
typedef BigInteger<uint64_t,0,1> uint128_ct;

/// literal operator
template<char ...digits>
consteval uint128_ct operator "" _num() noexcept
{
	return uint128_ct::to_number<digits...>();
}
}

namespace uint256_t {
typedef BigInteger<uint128_t,0,1> uint256_t;

/// literal operator
template<char ...digits>
consteval uint256_t operator "" _num() noexcept
{
	return uint256_t::to_number<digits...>();
}
}

namespace uint512_t {
typedef BigInteger<uint128_t,0,1,2,3> uint512_t;

/// literal operator
template<char ...digits>
consteval uint512_t operator "" _num() noexcept
{
	return uint512_t::to_number<digits...>();
}
}

namespace uint1024_t {
typedef BigInteger<uint128_t,0,1,2,3,4,5,6,7> uint1024_t;

/// literal operator
template<char ...digits>
consteval uint1024_t operator "" _num() noexcept
{
	return uint1024_t::to_number<digits...>();
}
}

}

#endif // BIGINTEGER_H
