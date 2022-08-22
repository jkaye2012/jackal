#pragma once

#include <cassert>
#include <cstdint>
#include <optional>
#include <string_view>
#include <variant>

#include "ast/builder.hpp"
#include "ast/node.hpp"

namespace jackal::ast
{
struct Constant : public AbstractSyntaxNode
{
  explicit constexpr Constant(int64_t constant) noexcept : _constant(constant) {}
  explicit constexpr Constant(double constant) noexcept : _constant(constant) {}

  [[nodiscard]] constexpr int64_t int_unsafe() const noexcept
  {
    return std::get<int64_t>(_constant);
  }
  [[nodiscard]] constexpr double double_unsafe() const noexcept
  {
    return std::get<double>(_constant);
  }

 private:
  std::variant<int64_t, double> _constant;
};

struct LocalVariable : public AbstractSyntaxNode
{
  explicit constexpr LocalVariable(std::string_view name) noexcept : _name(name) {}

  [[nodiscard]] constexpr std::string_view name() const noexcept { return _name; }

 private:
  std::string_view _name;
};

struct Value : public AbstractSyntaxNode
{
  struct Builder : public AstBuilder
  {
    Builder& set_constant(int64_t constant) noexcept
    {
      modified();
      _constant = Constant(constant);
      return *this;
    }

    Builder& set_constant(double constant) noexcept
    {
      modified();
      _constant = Constant(constant);
      return *this;
    }

    Builder& set_local(std::string_view local) noexcept
    {
      modified();
      _local = LocalVariable(local);
      return *this;
    }

    [[nodiscard]] Value build() const noexcept
    {
      assert(_constant.has_value() ^ _local.has_value());
      if (_constant.has_value())
      {
        return Value(*_constant);
      }

      return Value(*_local);
    }

   private:
    std::optional<Constant> _constant;
    std::optional<LocalVariable> _local;
  };

  explicit constexpr Value(Constant constant) noexcept : _value(std::move(constant)) {}
  explicit constexpr Value(LocalVariable local) noexcept : _value(std::move(local)) {}

  [[nodiscard]] constexpr Constant const& constant_unsafe() const noexcept
  {
    return std::get<Constant>(_value);
  }
  [[nodiscard]] constexpr LocalVariable const& local_variable_unsafe() const noexcept
  {
    return std::get<LocalVariable>(_value);
  }

 private:
  std::variant<Constant, LocalVariable> _value;
};
}  // namespace jackal::ast