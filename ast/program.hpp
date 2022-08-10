#pragma once

#include <variant>
#include <vector>

#include "ast/expression.hpp"

namespace jackal::ast
{
struct Instruction : public AbstractSyntaxNode
{
 private:
  std::variant<Binding, Print> _instr;
};

struct Program : public AbstractSyntaxNode
{
 private:
  std::vector<Instruction> _instructions;
};
}  // namespace jackal::ast
