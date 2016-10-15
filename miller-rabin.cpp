#define _CRT_SECURE_NO_DEPRECATE

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>

using namespace std;


long long abs (long long n)
{
	return n < 0 ? -n : n;
}

unsigned long long abs (unsigned long long n)
{
	return n;
}

template <class T>
bool even (const T & n)
{
	// return n % 2 == 0;
	return (n & 1) == 0;
}

template <class T>
void bisect (T & n)
{
	// n /= 2;
	n >>= 1;
}

template <class T>
void redouble (T & n)
{
	// n *= 2;
	n <<= 1;
}

template <class T>
bool perfect_square (const T & n)
{
	T sq = (T) ceil (sqrt ((double)n));
	return sq*sq == n;
}

template <class T>
T sq_root (const T & n)
{
	return (T) floor (sqrt ((double) n));
}

template <class T>
unsigned bits_in_number (T n)
{
	if (n == 0)
		return 1;
	unsigned result = 0;
	while (n)
	{
		bisect (n);
		++result;
	}
	return result;
}

template <class T>
bool test_bit (const T & n, unsigned k)
{
	return (n & (T(1) << k)) != 0;
}

template <class T>
void mulmod (T & a, T b, const T & n)
{
	a *= b;
	a %= n;
}

template <>
void mulmod (int & a, int b, const int & n)
{
	a = int( (((long long)a) * b) % n );
}

template <>
void mulmod (unsigned & a, unsigned b, const unsigned & n)
{
	a = unsigned( (((unsigned long long)a) * b) % n );
}

template <>
void mulmod (unsigned long long & a, unsigned long long b, const unsigned long long & n)
{
	if (a >= n)
		a %= n;
	if (b >= n)
		b %= n;
	unsigned long long res = 0;
	while (b)
		if (!even (b))
		{
			res += a;
			while (res >= n)
				res -= n;
			--b;
		}
		else
		{
			redouble (a);
			while (a >= n)
				a -= n;
			bisect (b);
		}
	a = res;
}

template <>
void mulmod (long long & a, long long b, const long long & n)
{
	bool neg = false;
	if (a < 0)
	{
		neg = !neg;
		a = -a;
	}
	if (b < 0)
	{
		neg = !neg;
		b = -b;
	}
	unsigned long long aa = a;
	mulmod<unsigned long long> (aa, (unsigned long long)b, (unsigned long long)n);
	a = (long long)aa * (neg ? -1 : 1);
}


template <class T, class T2>
T powmod (T a, T2 k, const T & n)
{
	T res = 1;
	while (k)
		if (!even (k))
		{
			mulmod (res, a, n);
			--k;
		}
		else
		{
			mulmod (a, a, n);
			bisect (k);
		}
	return res;
}

template <class T>
void transform_num (T n, T & p, T & q)
{
	T p_res = 0;
	while (even (n))
	{
		++p_res;
		bisect (n);
	}
	p = p_res;
	q = n;
}

template <class T, class T2>
T gcd (const T & a, const T2 & b)
{
	return (a == 0) ? b : gcd (b % a, a);
}

template <class T>
T jacobi (T a, T b)
{

#pragma warning (push)
#pragma warning (disable: 4146)

	if (a == 0)
		return 0;
	if (a == 1)
		return 1;

	if (a < 0)
		if ((b & 2) == 0)
			return jacobi (-a, b);
		else
			return - jacobi (-a, b);

	T e, a1;
	transform_num (a, e, a1);

	T s;
	if (even (e) || (b & 7) == 1 || (b & 7) == 7)
		s = 1;
	else
		s = -1;
	if ((b & 3) == 3 && (a1 & 3) == 3)
		s = -s;
	if (a1 == 1)
		return s;
	return s * jacobi (b % a1, a1);

#pragma warning (pop)

}

template <class T, class T2>
const std::vector<T> & get_primes (const T & b, T2 & pi)
{

	static std::vector<T> primes;
	static T counted_b;

	if (counted_b >= b)
		pi = T2 (std::upper_bound (primes.begin(), primes.end(), b) - primes.begin());
	else
	{
		if (counted_b == 0)
		{
			primes.push_back (2);
			counted_b = 2;
		}

		T first = counted_b == 2 ? 3 : primes.back()+2;
		for (T cur=first; cur<=b; ++++cur)
		{
			bool cur_is_prime = true;
			for (std::vector<T>::const_iterator iter = primes.begin(), end = primes.end();
				iter != end; ++iter)
			{
				const T & div = *iter;
				if (div * div > cur)
					break;
				if (cur % div == 0)
				{
					cur_is_prime = false;
					break;
				}
			}
			if (cur_is_prime)
				primes.push_back (cur);
		}

		counted_b = b;
		pi = (T2) primes.size();

	}

	return primes;

}

template <class T, class T2>
T2 prime_div_trivial (const T & n, T2 m)
{
	if (n == 2 || n == 3)
		return 1;
	if (n < 2)
		return 0;
	if (even (n))
		return 2;

	T2 pi;
	const vector<T2> & primes = get_primes (m, pi);

	for (std::vector<T2>::const_iterator iter=primes.begin(), end=primes.end();
		iter!=end && *iter <= m; ++iter)
	{
		const T2 & div = *iter;
		if (div * div > n)
			break;
		else
			if (n % div == 0)
				return div;
	}

	if (n < m*m)
		return 1;
	return 0;

}

template <class T, class T2>
bool miller_rabin (T n, T2 b)
{
	if (n == 2)
		return true;
	if (n < 2 || even (n))
		return false;

	if (b < 2)
		b = 2;
	for (T g; (g = gcd (n, b)) != 1; ++b)
		if (n > g)
			return false;

	T n_1 = n;
	--n_1;
	T p, q;
	transform_num (n_1, p, q);

	T rem = powmod (T(b), q, n);
	if (rem == 1 || rem == n_1)
		return true;

	for (T i=1; i<p; i++)
	{
		mulmod (rem, rem, n);
		if (rem == n_1)
			return true;
	}

	return false;

}

int main()
{
    return 0;
}










































