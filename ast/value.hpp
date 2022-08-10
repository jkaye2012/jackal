#pragma once

#include <cstdint>
#include <string_view>
#include <variant>

#include "ast/node.hpp"

namespace jackal::ast
{
struct Constant : public AbstractSyntaxNode
{
 private:
  std::variant<int64_t, double> _constant;
};

struct LocalVariable : public AbstractSyntaxNode
{
 private:
  std::string_view _name;
};

struct Value : public AbstractSyntaxNode
{
 private:
  std::variant<Constant, LocalVariable> _value;
};
}  // namespace jackal::ast
