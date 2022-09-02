#pragma once

#include <variant>
#include <vector>

#include "ast/expression.hpp"

namespace jackal::ast
{
struct Instruction : public AbstractSyntaxNode
{
  struct Builder
  {
    Binding::Builder binding;
    Print::Builder print;

    [[nodiscard]] constexpr bool was_modified() const noexcept
    {
      return binding.was_modified() || print.was_modified();
    }

    [[nodiscard]] Instruction build() const noexcept
    {
      assert(binding.was_modified() ^ print.was_modified());
      if (binding.was_modified())
      {
        return Instruction(binding.build());
      }

      return Instruction(print.build());
    }
  };

  explicit Instruction(Binding binding) noexcept : _instr(std::move(binding)) {}
  explicit Instruction(Print print) noexcept : _instr(std::move(print)) {}

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  [[nodiscard]] constexpr Binding const& binding_unsafe() const noexcept
  {
    return std::get<Binding>(_instr);
  }
  [[nodiscard]] constexpr Print const& print_unsafe() const noexcept
  {
    return std::get<Print>(_instr);
  }

 private:
  std::variant<Binding, Print> _instr;
};

struct Program : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  void add_instruction(Instruction instr) noexcept;

  [[nodiscard]] constexpr std::vector<Instruction> const& instructions() const noexcept
  {
    return _instructions;
  }

 private:
  std::vector<Instruction> _instructions;
};
}  // namespace jackal::ast
