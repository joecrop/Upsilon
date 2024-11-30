#include "integer_list_controller.h"
#include <poincare/based_integer.h>
#include <poincare/opposite.h>
#include <poincare/integer.h>
#include <poincare/decimal.h>
#include <poincare/float.h>
#include <poincare/logarithm.h>
#include <poincare/empty_layout.h>
#include <poincare/factor.h>
#include "../app.h"
#include "../../shared/poincare_helpers.h"

using namespace Poincare;
using namespace Shared;

namespace Calculation {

Integer::Base baseAtIndex(int index) {
  switch (index) {
    case 0:
      return Integer::Base::Decimal;
    case 1:
      return Integer::Base::Hexadecimal;
    default:
      assert(index == 2);
      return Integer::Base::Binary;
  }
}

void IntegerListController::setExpression(Poincare::Expression e) {
  ExpressionsListController::setExpression(e);
  static_assert(k_maxNumberOfRows >= k_indexOfFactorExpression + 1, "k_maxNumberOfRows must be greater than k_indexOfFactorExpression");
  assert(!m_expression.isUninitialized() && m_expression.type() == ExpressionNode::Type::BasedInteger || (m_expression.type() == ExpressionNode::Type::Opposite && m_expression.childAtIndex(0).type() == ExpressionNode::Type::BasedInteger));
  assert(!m_expression.isUninitialized());

  if (m_expression.type() == ExpressionNode::Type::BasedInteger) {
    Integer integer = static_cast<BasedInteger &>(m_expression).integer();
    for (int index = 0; index < k_indexOfEngineeringExpression; ++index) {
      m_layouts[index] = integer.createLayout(baseAtIndex(index));
    }
  }
  else
  {
    Opposite b = static_cast<Opposite &>(m_expression);
    Expression e = b.childAtIndex(0);
    Integer childInt = static_cast<BasedInteger &>(e).integer();
    childInt.setNegative(true);
    Integer num_bits = Integer::CeilingLog2(childInt);
    Integer integer = Integer::TwosComplementToBits(childInt, num_bits);
    for (int index = 0; index < k_indexOfEngineeringExpression; ++index) {
      if(baseAtIndex(index) == Integer::Base::Decimal) {
        m_layouts[index] = childInt.createLayout(baseAtIndex(index));
      } else {
        m_layouts[index] = integer.createLayout(baseAtIndex(index));
      }
    }
  }
  // Computing engineering notation
  Integer integer = static_cast<BasedInteger &>(m_expression).integer();
  double dub = integer.approximate<double>();
  Float<double> f = Float<double>::Builder(dub);
  m_layouts[k_indexOfEngineeringExpression] = f.createLayout(Preferences::PrintFloatMode::Engineering, Preferences::MediumNumberOfSignificantDigits);
  // Computing factorExpression
  Expression factor = Factor::Builder(m_expression.clone());
  PoincareHelpers::Simplify(&factor, App::app()->localContext(), ExpressionNode::ReductionTarget::User);
  if (!factor.isUndefined()) {
    m_layouts[k_indexOfFactorExpression] = PoincareHelpers::CreateLayout(factor);
  }
}

I18n::Message IntegerListController::messageAtIndex(int index) {
  switch (index) {
    case 0:
      return I18n::Message::DecimalBase;
    case 1:
      return I18n::Message::HexadecimalBase;
    case 2:
      return I18n::Message::BinaryBase;
    case 3:
      return I18n::Message::Engineering;
    default:
      return I18n::Message::PrimeFactors;
  }
}

}
