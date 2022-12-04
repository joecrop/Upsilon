#include "integer_list_controller.h"
#include <poincare/based_integer.h>
#include <poincare/opposite.h>
#include <poincare/integer.h>
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
  assert(!m_expression.isUninitialized() && m_expression.type() == ExpressionNode::Type::BasedInteger);
  assert(!m_expression.isUninitialized());

  if (m_expression.type() == ExpressionNode::Type::BasedInteger) {
    Integer integer = static_cast<BasedInteger &>(m_expression).integer();
    for (int index = 0; index < k_indexOfFactorExpression; ++index) {
      m_layouts[index] = integer.createLayout(baseAtIndex(index));
    }
  }
  else
  {
    Opposite b = static_cast<Opposite &>(m_expression);
    Expression e = b.childAtIndex(0);
    Integer childInt = static_cast<BasedInteger &>(e).integer();
    Integer integer = Integer::Multiplication(childInt, Integer(-1));
    m_layouts[0] = integer.createLayout(baseAtIndex(0));
  }
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
    default:
      return I18n::Message::PrimeFactors;
  }
}

}
