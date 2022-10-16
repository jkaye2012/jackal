#pragma once

#include <memory>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Context; }
namespace jackal::ast { struct TypeIdentifier; }
// clang-format on

namespace jackal::ast
{
struct Type : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Type() override;
  Type(Type const&) = delete;
  Type& operator=(Type const&) = delete;
  Type(Type&&) noexcept = delete;
  Type& operator=(Type&&) noexcept = delete;

  TypeIdentifier& name() noexcept;

  Context& context() noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
