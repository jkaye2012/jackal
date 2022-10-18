#pragma once

// clang-format off
namespace jackal::ast { struct Arguments; }
namespace jackal::ast { struct Context; }
namespace jackal::ast { struct Executable; }
namespace jackal::ast { struct Expressions; }
namespace jackal::ast { struct Function; }
namespace jackal::ast { struct FunctionCall; }
namespace jackal::ast { struct Number; }
namespace jackal::ast { struct Parameters; }
namespace jackal::ast { struct Primitive; }
namespace jackal::ast { struct Scope; }
namespace jackal::ast { struct Type; }
namespace jackal::ast { struct Variable; }
namespace jackal::ast { struct ValueV1; }


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

  virtual void visit(Arguments& node) noexcept = 0;
  virtual void visit(Context& node) noexcept = 0;
  virtual void visit(Executable& node) noexcept = 0;
  virtual void visit(Expressions& node) noexcept = 0;
  virtual void visit(Function& node) noexcept = 0;
  virtual void visit(FunctionCall& node) noexcept = 0;
  virtual void visit(Number& node) noexcept = 0;
  virtual void visit(Parameters& node) noexcept = 0;
  virtual void visit(Primitive& node) noexcept = 0;
  virtual void visit(Scope& node) noexcept = 0;
  virtual void visit(Type& node) noexcept = 0;
  virtual void visit(Variable& node) noexcept = 0;
  virtual void visit(ValueV1& node) noexcept = 0;

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
