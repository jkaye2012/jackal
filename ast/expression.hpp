#pragma once

#include <variant>

#include "ast/operator.hpp"
#include "ast/value.hpp"

namespace jackal::ast
{
struct Expression : public AbstractSyntaxNode
{
 private:
  std::variant<Operator, Value> _expr;
};

struct Binding : public AbstractSyntaxNode
{
 private:
  LocalVariable _variable;
  Expression _expr;
};

struct Print : public AbstractSyntaxNode
{
 private:
  Expression _expr;
};
}  // namespace jackal::ast
