#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>
#include <variant>

#include "ast/builder.hpp"
#include "ast/node.hpp"
#include "ast/visitor.hpp"

namespace jackal::ast
{
struct Constant : public AbstractSyntaxNode
{
  explicit Constant(int64_t constant) noexcept;
  explicit Constant(double constant) noexcept;

  ~Constant() override;
  Constant(Constant const&) = delete;
  Constant& operator=(Constant const&) = delete;
  Constant(Constant&&) noexcept;
  Constant& operator=(Constant&&) noexcept;

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  [[nodiscard]] int64_t int_unsafe() const noexcept;

  [[nodiscard]] double double_unsafe() const noexcept;

  [[nodiscard]] std::variant<int64_t, double>& constant() noexcept;

  [[nodiscard]] std::variant<int64_t, double> const& constant() const noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};

struct LocalVariable : public AbstractSyntaxNode
{
  explicit LocalVariable(std::string_view name) noexcept;

  ~LocalVariable() override;
  LocalVariable(LocalVariable const&) = delete;
  LocalVariable& operator=(LocalVariable const&) = delete;
  LocalVariable(LocalVariable&&) noexcept;
  LocalVariable& operator=(LocalVariable&&) noexcept;

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  [[nodiscard]] std::string_view name() const noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
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

    [[nodiscard]] Value build() noexcept
    {
      assert(_constant.has_value() ^ _local.has_value());
      if (_constant.has_value())
      {
        return Value(std::move(*_constant));
      }

      return Value(std::move(*_local));
    }

   private:
    std::optional<Constant> _constant;
    std::optional<LocalVariable> _local;
  };

  explicit Value(Constant constant) noexcept;
  explicit Value(LocalVariable local) noexcept;

  ~Value() override;
  Value(Value const&) = delete;
  Value& operator=(Value const&) = delete;
  Value(Value&&) noexcept;
  Value& operator=(Value&&) noexcept;

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  [[nodiscard]] Constant const& constant_unsafe() const noexcept;

  [[nodiscard]] LocalVariable const& local_variable_unsafe() const noexcept;

  [[nodiscard]] std::variant<Constant, LocalVariable>& value() noexcept;

  [[nodiscard]] std::variant<Constant, LocalVariable> const& value() const noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
