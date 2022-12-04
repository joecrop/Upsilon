#include <poincare/preferences.h>
#include <poincare/rational.h>
#include "helper.h"

using namespace Poincare;

static inline void assert_equal(const Rational i, const Rational j)
{
  quiz_assert(Rational::NaturalOrder(i, j) == 0);
}

static inline void assert_not_equal(const Integer i, const Integer j)
{
  quiz_assert(Integer::NaturalOrder(i, j) != 0);
}

static inline void assert_lower(const Integer i, const Integer j)
{
  quiz_assert(Integer::NaturalOrder(i, j) < 0);
}

static inline void assert_greater(const Integer i, const Integer j)
{
  quiz_assert(Integer::NaturalOrder(i, j) > 0);
}

QUIZ_CASE(poincare_fixed_point_compare)
{
  Preferences::sharedPreferences()->setNumberOfFixedPointDigits(8);
  Integer den = Integer(1);
  Integer a = Integer(255);
  assert_equal(Rational::Builder(a, den, true), Rational::Builder(-1));
  a = Integer(127);
  assert_equal(Rational::Builder(a, den, true), Rational::Builder(127));
  a = Integer(128);
  assert_equal(Rational::Builder(a, den, true), Rational::Builder(-128));
  a = Integer(16);
  assert_equal(Rational::Builder(a, den, true), Rational::Builder(16));
  a = Integer(-16);
  assert_equal(Rational::Builder(a, den, true), Rational::Builder(-16));
  a = Integer(0);
  a.setNegative(true);
  assert_equal(Rational::Builder(a, den, true), Rational::Builder(0));
  a = Integer(169); // = 16.9
  den = Integer(10);
  assert_equal(Rational::Builder(a, den, true), Rational::Builder(16));

  a = Integer(2);
  den = Integer(3);
  assert_equal(Rational::Builder(a, den, true), Rational::Builder(0));

  a = Integer(3);
  den = Integer(2);
  assert_equal(Rational::Builder(a, den, true), Rational::Builder(1));

  Preferences::sharedPreferences()->setNumberOfFixedPointDigits(0);
}

static inline void assert_add_to(const Rational i, const Rational j, const Rational k)
{
  quiz_assert(Rational::NaturalOrder(Rational::Addition(i, j), k) == 0);
}

QUIZ_CASE(poincare_fixed_point_addition)
{
  Preferences::sharedPreferences()->setNumberOfFixedPointDigits(8);
  Integer den = Integer(1);
  Integer a = Integer(1);
  Integer b = Integer(1);
  assert_add_to(Rational::Builder(a, den, true), Rational::Builder(b, den, true), Rational::Builder(2));

  a = Integer(10);
  b = Integer(20);
  assert_add_to(Rational::Builder(a, den, true), Rational::Builder(b, den, true), Rational::Builder(30));

  a = Integer(127);
  b = Integer(1);
  assert_add_to(Rational::Builder(a, den, true), Rational::Builder(b, den, true), Rational::Builder(-128));

  a = Integer(-127);
  b = Integer(-2);
  assert_add_to(Rational::Builder(a, den, true), Rational::Builder(b, den, true), Rational::Builder(127));

  Preferences::sharedPreferences()->setNumberOfFixedPointDigits(0);
}

static inline void assert_mult_to(const Rational i, const Rational j, const Rational k)
{
  quiz_assert(Rational::NaturalOrder(Rational::Multiplication(i, j), k) == 0);
}

QUIZ_CASE(poincare_fixed_point_multiplication)
{
  Preferences::sharedPreferences()->setNumberOfFixedPointDigits(8);

  Integer den = Integer(1);
  Integer a = Integer(1);
  Integer b = Integer(1);
  assert_mult_to(Rational::Builder(a, den, true), Rational::Builder(b, den, true), Rational::Builder(1));

  a = Integer(30);
  b = Integer(-2);
  assert_mult_to(Rational::Builder(a, den, true), Rational::Builder(b, den, true), Rational::Builder(-60));

  a = Integer(64);
  b = Integer(2);
  assert_mult_to(Rational::Builder(a, den, true), Rational::Builder(b, den), Rational::Builder(-128));

  a = Integer(64);
  b = Integer(-2);
  assert_mult_to(Rational::Builder(a, den), Rational::Builder(b, den, true), Rational::Builder(-128));

  Preferences::sharedPreferences()->setNumberOfFixedPointDigits(0);
}

static inline void assert_pow_to(const Rational i, const Integer j, const Rational k)
{
  quiz_assert(Rational::NaturalOrder(Rational::IntegerPower(i, j), k) == 0);
}

QUIZ_CASE(poincare_fixed_point_pow)
{
  Preferences::sharedPreferences()->setNumberOfFixedPointDigits(8);

  Integer den = Integer(1);
  Integer a = Integer(2);
  assert_pow_to(Rational::Builder(a, den, true), Integer(2), Rational::Builder(4));
  a = Integer(3);
  assert_pow_to(Rational::Builder(a, den, true), Integer(4), Rational::Builder(81));
  a = Integer(3);
  assert_pow_to(Rational::Builder(a, den, true), Integer(5), Rational::Builder(-13));

  Preferences::sharedPreferences()->setNumberOfFixedPointDigits(0);
}