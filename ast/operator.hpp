#pragma once

#include <optional>

#include "ast/builder.hpp"
#include "ast/value.hpp"

namespace jackal::ast
{
struct Operator : public AbstractSyntaxNode
{
  enum class Type
  {
    Add
  };

  struct Builder : public AstBuilder
  {
    Builder& set_type(Type type) noexcept
    {
      modified();
      _type = type;
      return *this;
    }

    Builder& set_a(Value a) noexcept
    {
      modified();
      _a = std::move(a);
      return *this;
    }

    Builder& set_b(Value b) noexcept
    {
      modified();
      _b = std::move(b);
      return *this;
    }

    [[nodiscard]] Operator build() const noexcept
    {
      return Operator(_type.value(), _a.value(), _b.value());
    }

   private:
    std::optional<Type> _type;
    std::optional<Value> _a;
    std::optional<Value> _b;
  };

  Operator(Type type, Value a, Value b) noexcept : _type(type), _a(std::move(a)), _b(std::move(b))
  {
  }

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  [[nodiscard]] Type type() const noexcept { return _type; }

  [[nodiscard]] Value& a() noexcept { return _a; }
  [[nodiscard]] Value const& a() const noexcept { return _a; }

  [[nodiscard]] Value& b() noexcept { return _b; }
  [[nodiscard]] Value const& b() const noexcept { return _b; }

 private:
  Type _type;
  Value _a;
  Value _b;
};
}  // namespace jackal::ast
