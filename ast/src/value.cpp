#include "ast/value.hpp"

#include <memory>
#include <string_view>
#include <variant>

using jackal::ast::Constant;
using jackal::ast::LocalVariable;
using jackal::ast::Value;

struct Constant::Impl
{
  std::variant<int64_t, double> constant;
};

Constant::Constant(int64_t constant) noexcept : _impl(std::make_unique<Impl>(Impl{constant})) {}

Constant::Constant(double constant) noexcept : _impl(std::make_unique<Impl>(Impl{constant})) {}

Constant::~Constant() = default;
Constant::Constant(Constant&&) noexcept = default;
Constant& Constant::operator=(Constant&&) noexcept = default;

auto Constant::int_unsafe() const noexcept -> int64_t { return std::get<int64_t>(_impl->constant); }

auto Constant::double_unsafe() const noexcept -> double
{
  return std::get<double>(_impl->constant);
}

auto Constant::constant() noexcept -> std::variant<int64_t, double>& { return _impl->constant; }

auto Constant::constant() const noexcept -> std::variant<int64_t, double> const&
{
  return _impl->constant;
}

struct LocalVariable::Impl
{
  std::string_view name;
};

LocalVariable::LocalVariable(std::string_view name) noexcept
    : _impl(std::make_unique<Impl>(Impl{name}))
{
}

LocalVariable::~LocalVariable() = default;
LocalVariable::LocalVariable(LocalVariable&&) noexcept = default;
LocalVariable& LocalVariable::operator=(LocalVariable&&) noexcept = default;

auto LocalVariable::name() const noexcept -> std::string_view { return _impl->name; }

struct Value::Impl
{
  std::variant<Constant, LocalVariable> value;
};

Value::Value(Constant constant) noexcept : _impl(std::make_unique<Impl>(Impl{std::move(constant)}))
{
}

Value::Value(LocalVariable local) noexcept : _impl(std::make_unique<Impl>(Impl{std::move(local)}))
{
}

Value::~Value() = default;
Value::Value(Value&&) noexcept = default;
Value& Value::operator=(Value&&) noexcept = default;

auto Value::constant_unsafe() const noexcept -> Constant const&
{
  return std::get<Constant>(_impl->value);
}

auto Value::local_variable_unsafe() const noexcept -> LocalVariable const&
{
  return std::get<LocalVariable>(_impl->value);
}

auto Value::value() noexcept -> std::variant<Constant, LocalVariable>& { return _impl->value; };

auto Value::value() const noexcept -> std::variant<Constant, LocalVariable> const&
{
  return _impl->value;
};
