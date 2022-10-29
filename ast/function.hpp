#pragma once

#include <memory>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Arguments; }
namespace jackal::ast { struct Context; }
namespace jackal::ast { struct Scope; }
namespace jackal::ast { struct Type; }
namespace jackal::ast { struct ValueIdentifier; }
// clang-format on

namespace jackal::ast
{
struct Function : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  Function(ValueIdentifier name, Context ctx, Arguments args, Type ret, Scope body) noexcept;

  ~Function() override;
  Function(Function const&) = delete;
  Function& operator=(Function const&) = delete;
  Function(Function&&) noexcept;
  Function& operator=(Function&&) noexcept;

  ValueIdentifier& name() noexcept;

  Context& context() noexcept;

  Arguments& args() noexcept;

  Type& return_type() noexcept;

  Scope& body() noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
