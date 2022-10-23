#pragma once

#include <memory>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Type; }
namespace jackal::ast { struct ValueIdentifier; }
// clang-format on

namespace jackal::ast
{
struct PropertyAccess : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~PropertyAccess() override;
  PropertyAccess(PropertyAccess const&) = delete;
  PropertyAccess& operator=(PropertyAccess const&) = delete;
  PropertyAccess(PropertyAccess&&) noexcept;
  PropertyAccess& operator=(PropertyAccess&&) noexcept;

  Type& type() noexcept;

  ValueIdentifier& target_name() noexcept;

  ValueIdentifier& property_name() noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
