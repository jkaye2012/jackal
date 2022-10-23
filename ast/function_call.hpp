#pragma once

#include <memory>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Context; }
namespace jackal::ast { struct Parameters; }
namespace jackal::ast { struct Type; }
namespace jackal::ast { struct ValueIdentifier; }
// clang-format on

namespace jackal::ast
{
struct FunctionCall : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~FunctionCall() override;
  FunctionCall(FunctionCall const&) = delete;
  FunctionCall& operator=(FunctionCall const&) = delete;
  FunctionCall(FunctionCall&&) noexcept = delete;
  FunctionCall& operator=(FunctionCall&&) noexcept = delete;

  ValueIdentifier& name() noexcept;

  Context& context() noexcept;

  Parameters& params() noexcept;

  Type& type() noexcept;

  // TODO: extract Function given an Environment (allowing access to Type, Arguments, etc.)

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
