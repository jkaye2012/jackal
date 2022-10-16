#pragma once

#include <memory>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Arguments; }
namespace jackal::ast { struct Scope; }
namespace jackal::ast { struct Type; }
namespace jackal::ast { struct ValueIdentifier; }
// clang-format on

namespace jackal::ast
{
struct Function : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Function() override;
  Function(Function const&) = delete;
  Function& operator=(Function const&) = delete;
  Function(Function&&) noexcept = delete;
  Function& operator=(Function&&) noexcept = delete;

  ValueIdentifier& name() noexcept;

  Arguments& args() noexcept;

  Type& return_type() noexcept;

  Scope& body() noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
