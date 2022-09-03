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

    [[nodiscard]] bool was_modified() const noexcept
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

  explicit Instruction(Binding binding) noexcept : instruction(std::move(binding)) {}
  explicit Instruction(Print print) noexcept : instruction(std::move(print)) {}

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  [[nodiscard]] Binding const& binding_unsafe() const noexcept
  {
    return std::get<Binding>(instruction);
  }

  [[nodiscard]] Print const& print_unsafe() const noexcept { return std::get<Print>(instruction); }

  std::variant<Binding, Print> instruction;
};

struct Program : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  void add_instruction(Instruction instr) noexcept;

  std::vector<Instruction> instructions;
};
}  // namespace jackal::ast
