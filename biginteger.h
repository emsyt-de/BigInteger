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
#include <iomanip>
#include <iostream>

namespace biginteger {

inline std::ostream & operator<<(std::ostream & stream, const __uint128_t & r)
{
	uint64_t l = static_cast<uint64_t>(r);
	uint64_t u = r>>64;
	if(u > 0)
		return stream<<std::showbase<<u<<std::noshowbase<<std::setfill('0')<<std::setw(16)<<l<<std::showbase;
	else
		return stream<<std::showbase<<l;

}

template<std::unsigned_integral B, bool is_signed, std::size_t ...I>
class BigInteger
{

	friend class BigInteger<B,!is_signed,I...>;

public:
	/// Constructors
	constexpr BigInteger() = default;
	constexpr BigInteger(const BigInteger & n) = default;
	constexpr BigInteger(BigInteger && n) = default;

	template<std::unsigned_integral T>
	constexpr BigInteger(const T & n)
		: numbers{n}
	{}

	template<std::signed_integral T>
	constexpr BigInteger(const T & n)
	{
		if(n < 0)
		{
			std::fill(numbers.begin(),numbers.end(),std::numeric_limits<B>::max());
			numbers[0] -= (n + 1);
		}
		else
		{
			numbers[0] = n;
		}
	}

	/// Convert signedness copy constructor
	constexpr BigInteger(const BigInteger<B,!is_signed,I...> & n)
		: numbers(n.numbers)
	{}

	/// Convert signedness move constructor
	constexpr BigInteger(BigInteger<B,!is_signed,I...> && n)
		: numbers(std::forward<decltype(n.numbers)>(n.numbers))
	{}

	/// Assignment Operators
	BigInteger & operator=(const BigInteger & n) = delete;
	constexpr BigInteger & operator=(BigInteger && n) = default;

protected:
	constexpr BigInteger(std::array<B,sizeof... (I)>&& arr)
		: numbers(std::forward<std::array<B,sizeof... (I)>>(arr))
	{}

public:
	/// Typecast Operators

	/// explicit keyword resolves ambiguous problem with convert constructor
	explicit constexpr operator bool() const
	{
		return (numbers[I] || ...);
	}

//	explicit constexpr operator BigInteger<B,!is_signed,I...>() const
//	{
//		return numbers;
//	}

	/// Bitwise Operators

	/// And
	friend inline constexpr BigInteger operator&(const BigInteger & l, const BigInteger & r)
	{
		return {{(l.numbers[I] & r.numbers[I])...}};
	}

	friend inline constexpr BigInteger& operator&=(BigInteger & l, const BigInteger & r)
	{
		return l = (l & r);
	}

	/// Or
	friend inline constexpr BigInteger operator|(const BigInteger & l, const BigInteger & r)
	{
		return {{(l.numbers[I] | r.numbers[I]) ...}};
	}

	friend inline constexpr BigInteger& operator|=(BigInteger & l, const BigInteger & r)
	{
		return l = (l | r);
	}

	/// Xor
	friend inline constexpr BigInteger operator^(const BigInteger & l, const BigInteger & r)
	{
		return {{(l.numbers[I] ^ r.numbers[I]) ...}};
	}

	friend inline constexpr BigInteger& operator^=(BigInteger & l, const BigInteger & r)
	{
		return l = (l ^ r);
	}

	/// Invert operator
	inline constexpr BigInteger operator~() const
	{
		return {{(~numbers[I])...}};
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
		if constexpr(is_signed)
		{
			((ret = (std::strong_ordering::equal != l.numbers[I] <=> r.numbers[I]) ?
					(I + 1 < sizeof...(I) ? l.numbers[I] <=> r.numbers[I] : std::make_signed_t<B>(l.numbers[I]) <=> std::make_signed_t<B>(r.numbers[I])) : ret), ...);
		}
		else
		{
			((ret = (std::strong_ordering::equal != l.numbers[I] <=> r.numbers[I]) ? l.numbers[I] <=> r.numbers[I] : ret), ...);
		}
		return ret;
	}

	/// Arithmetic Operators

	/// Plus
	friend inline constexpr BigInteger operator+(const BigInteger & l, const BigInteger & r)
	{
		BigInteger sum{{(std::get<I>(l.numbers) + std::get<I>(r.numbers))...}};
		BigInteger carry{{(sum.numbers[I] < l.numbers[I])...}};
		auto temp = sum;
		((std::get<I>(sum.numbers) += I > 0  ? carry.numbers[I-1] : 0), ...);
		BigInteger carry1{{(sum.numbers[I] < temp.numbers[I])...}};
		((std::get<I>(sum.numbers) += I > 0 ? carry1.numbers[I-1] : 0), ...);
		return sum;
	}

	friend inline constexpr BigInteger & operator+=(BigInteger & l, const BigInteger & r)
	{
		return l = l + r;
	}

	/// Minus
	friend constexpr inline BigInteger operator-(const BigInteger & l, const BigInteger & r)
	{
		BigInteger diff{{(std::get<I>(l.numbers) - std::get<I>(r.numbers))...}};
		BigInteger carry{{(diff.numbers[I] > l.numbers[I])...}};
		auto temp = diff;
		((std::get<I>(diff.numbers) -= I > 0 ? carry.numbers[I-1] : 0), ...);
		BigInteger carry1{{(diff.numbers[I] > temp.numbers[I])...}};
		((std::get<I>(diff.numbers) -= I > 0 ? carry1.numbers[I-1] : 0), ...);
		return diff;
	}


	friend inline constexpr BigInteger & operator-=(BigInteger & l, const BigInteger & r){
		return l = l - r;
	}

	/// Multiply
	friend inline constexpr BigInteger operator*(const BigInteger & l, const BigInteger & r)
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

	friend inline constexpr BigInteger & operator*=(BigInteger & l, const BigInteger & r){
		return l = l * r;
	}

	/// Divide
	friend inline constexpr BigInteger operator/(const BigInteger & l, const BigInteger & r){
		return divmod(l,r).first;
	}

	friend inline constexpr BigInteger & operator/=(BigInteger & l, const BigInteger & r){
		return l = l / r;
	}

	/// Modulo
	friend inline constexpr BigInteger operator%(const BigInteger & l, const BigInteger & r){
		return divmod(l,r).second;
	}

	friend inline constexpr BigInteger & operator%=(BigInteger & l, const BigInteger & r){
		return l = l % r;
	}

	/// Bit Shift Operators

	/// Shift left
	template <std::unsigned_integral T2>
	friend inline constexpr BigInteger operator<<(const BigInteger & l, const T2 & r)
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

	template <std::unsigned_integral T2>
	friend inline constexpr BigInteger& operator<<=(BigInteger & l, const T2 & r)
	{
		return l = l << r;
	}

	/// Shift right
	template <std::unsigned_integral T2>
	friend inline constexpr BigInteger operator>>(const BigInteger & l, const T2 & r)
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

	template <std::unsigned_integral T2>
	friend inline constexpr BigInteger& operator>>=(BigInteger & l, const T2 & r)
	{
		return l = l >> r;
	}

	/// Increment Operators

	/// pre increment
	friend BigInteger & operator++(BigInteger & hs)
	{
		hs += 1u;
		return hs;
	}

	/// post increment
	friend BigInteger operator++(BigInteger & hs, int)
	{
		auto temp(hs);
		hs += 1u;
		return temp;
	}

	/// pre decrement
	friend BigInteger & operator--(BigInteger & hs)
	{
		hs -= 1u;
		return hs;
	}

	/// post decrement
	friend BigInteger operator--(BigInteger & hs, int)
	{
		auto temp(hs);
		hs -= 1u;
		return temp;
	}

	/// positive sign operator
	friend BigInteger operator+(const BigInteger & hs)
	{
		return hs;
	}

	/// negative sign operator
	friend BigInteger operator-(const BigInteger & hs)
	{
		static constexpr BigInteger uint256_1(1);
		return ~hs + uint256_1;
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
				else if constexpr(sizeof (B) == sizeof (uint64_t))
				{
					out -= static_cast<std::size_t>(__builtin_clzll(*it));
				}
				else if constexpr(sizeof (B) == sizeof (uint32_t))
				{
					out -= static_cast<std::size_t>(__builtin_clz(*it));
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

	static consteval BigInteger max() noexcept
	{
		BigInteger max({(I,std::numeric_limits<B>::max())...});
		if constexpr(is_signed)
		{
			// clear msb bit
			max.numbers[sizeof... (I) - 1] >>= 1;
		}
		return max;
	}

	static consteval BigInteger min() noexcept
	{
		BigInteger min; // default initalized to 0
		if constexpr(is_signed)
		{
			// set msb bit
			min.numbers[sizeof... (I) - 1] |= (B(1) << (sizeof (B)*8 - 1));
		}
		return min;
	}

	template<std::unsigned_integral T>
	static constexpr BigInteger exp(BigInteger&& x, T n)
	{
		return exp_by_squaring(1u, std::forward<BigInteger>(x), n);
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

	static constexpr std::size_t bit_size = sizeof(B) * sizeof... (I) * 8;

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
		BigInteger one(1u);
		BigInteger zero(0u);
		auto msb_r = bits(r);
		if(r == zero)
		{
			throw std::invalid_argument("Devide with zero!");
		}
		else if(l < r)
		{
			/// use copy constructor for l
			return std::make_pair(0u,l);
		}
		else if (one << msb_r == r)  // check if r is from type 2^n
		{
			return std::make_pair(l>>msb_r, l & (r - 1u));
		}
		auto msb_l = bits(l);
		auto diff_msb = msb_l - msb_r;
		/// use copy constructor for l
		BigInteger ll = l;
		BigInteger rr = r << diff_msb;
		BigInteger i = one << diff_msb;
		BigInteger div;
		while(i)
		{
			if(ll >= rr)
			{
				div |= i;
				ll -= rr;
			}
			rr >>= 1u;
			i >>= 1u;
		}
		return std::make_pair(std::forward<BigInteger>(div),std::forward<BigInteger>(ll));
	}

	/// {} makes value initialization for non class non array types like int to its default value 0
	std::array<B,sizeof... (I)> numbers{};
};

namespace uint128 {
typedef BigInteger<uint64_t,false,0,1> uint128_t;

/// literal operator
template<char ...digits>
constexpr uint128_t operator "" _num() noexcept
{
	return uint128_t::to_number<digits...>();
}
}

namespace uint256 {
typedef BigInteger<__uint128_t,false,0,1> uint256_t;

/// literal operator
template<char ...digits>
constexpr uint256_t operator "" _num() noexcept
{
	return uint256_t::to_number<digits...>();
}
}

namespace uint512 {
typedef BigInteger<__uint128_t,false,0,1,2,3> uint512_t;

/// literal operator
template<char ...digits>
constexpr uint512_t operator "" _num() noexcept
{
	return uint512_t::to_number<digits...>();
}
}

namespace int512 {

typedef BigInteger<__uint128_t,true,0,1,2,3> int512_t;

template<char ...digits>
constexpr int512_t operator "" _num() noexcept
{
	return int512_t::to_number<digits...>();
}

}

namespace uint1024 {
typedef BigInteger<__uint128_t,false,0,1,2,3,4,5,6,7> uint1024_t;

/// literal operator
template<char ...digits>
constexpr uint1024_t operator "" _num() noexcept
{
	return uint1024_t::to_number<digits...>();
}
}

using uint128_t = uint128::uint128_t;
using uint256_t = uint256::uint256_t;
using uint512_t = uint512::uint512_t;
using int512_t = int512::int512_t;
using uint1024_t = uint1024::uint1024_t;

}

namespace std {
template <>
struct is_integral<biginteger::int512_t> : std::true_type {};
template <>
struct is_integral<biginteger::uint512_t> : std::true_type {};
}

#endif // BIGINTEGER_H
