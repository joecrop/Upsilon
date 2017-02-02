#include <poincare/addition.h>
extern "C" {
#include <assert.h>
#include <stdlib.h>
}
#include "layout/horizontal_layout.h"
#include "layout/string_layout.h"
#include "layout/parenthesis_layout.h"

Expression::Type Addition::type() const {
  return Type::Addition;
}

ExpressionLayout * Addition::createLayout(DisplayMode displayMode) const {
  ExpressionLayout** children_layouts = (ExpressionLayout **)malloc(3*sizeof(ExpressionLayout *));
  children_layouts[0] = m_operands[0]->createLayout(displayMode);
  children_layouts[1] = new StringLayout("+", 1);
  children_layouts[2] = m_operands[1]->type() == Type::Opposite ? new ParenthesisLayout(m_operands[1]->createLayout(displayMode)) : m_operands[1]->createLayout(displayMode);
  return new HorizontalLayout(children_layouts, 3);
}

float Addition::approximate(Context& context, AngleUnit angleUnit) const {
  return m_operands[0]->approximate(context, angleUnit)+m_operands[1]->approximate(context, angleUnit);;
}

Expression * Addition::cloneWithDifferentOperands(Expression** newOperands,
        int numberOfOperands, bool cloneOperands) const {
  return new Addition(newOperands, cloneOperands);
}

bool Addition::isCommutative() const {
  return true;
}

Expression * Addition::evaluateOnComplex(Complex * c, Complex * d, Context& context, AngleUnit angleUnit) const {
  return new Complex(c->a()+ d->a(), c->b() + d->b());
}
