#pragma once

#include "ast/value.hpp"

namespace jackal::ast
{
struct Operator : public AbstractSyntaxNode
{
  enum class Type
  {
    Add
  };

 private:
  Type _type;
  Value _a;
  Value _b;
};
}  // namespace jackal::ast
