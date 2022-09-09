#pragma once

#include <memory>
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

    [[nodiscard]] Instruction build() noexcept
    {
      assert(binding.was_modified() ^ print.was_modified());
      if (binding.was_modified())
      {
        return Instruction(binding.build());
      }

      return Instruction(print.build());
    }
  };

  explicit Instruction(Binding binding) noexcept;
  explicit Instruction(Print print) noexcept;

  ~Instruction() override;
  Instruction(Instruction const&) = delete;
  Instruction& operator=(Instruction const&) = delete;
  Instruction(Instruction&&) noexcept;
  Instruction& operator=(Instruction&&) noexcept;

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  [[nodiscard]] Binding const& binding_unsafe() const noexcept;

  [[nodiscard]] Print const& print_unsafe() const noexcept;

  [[nodiscard]] std::variant<Binding, Print>& instruction() noexcept;
  [[nodiscard]] std::variant<Binding, Print> const& instruction() const noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};

struct Program : public AbstractSyntaxNode
{
  Program() noexcept;

  ~Program() override;
  Program(Program const&) = delete;
  Program& operator=(Program const&) = delete;
  Program(Program&&) noexcept;
  Program& operator=(Program&&) noexcept;

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  void add_instruction(Instruction instr) noexcept;

  [[nodiscard]] std::vector<Instruction>& instructions() noexcept;
  [[nodiscard]] std::vector<Instruction> const& instructions() const noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
