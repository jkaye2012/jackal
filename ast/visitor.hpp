#pragma once

// clang-format off
namespace jackal::ast { struct Operator; }

namespace jackal::ast { struct Expression; }
namespace jackal::ast { struct Binding; }
namespace jackal::ast { struct Print; }

namespace jackal::ast { struct Instruction; }
namespace jackal::ast { struct Program; }

namespace jackal::ast { struct Value; }
namespace jackal::ast { struct Constant; }
namespace jackal::ast { struct LocalVariable; }
// clang-format on

namespace jackal::ast
{
struct Visitor
{
  virtual ~Visitor() = default;

  virtual void visit(Operator& node) noexcept = 0;

  virtual void visit(Expression& node) noexcept = 0;
  virtual void visit(Binding& node) noexcept = 0;
  virtual void visit(Print& node) noexcept = 0;

  virtual void visit(Instruction& node) noexcept = 0;
  virtual void visit(Program& node) noexcept = 0;

  virtual void visit(Value& node) noexcept = 0;
  virtual void visit(Constant& node) noexcept = 0;
  virtual void visit(LocalVariable& node) noexcept = 0;
};
}  // namespace jackal::ast
