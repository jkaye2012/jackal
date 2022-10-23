#pragma once

#include <memory>
#include <variant>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Type; }
namespace jackal::ast { struct ValueIdentifier; }
// clang-format on

namespace jackal::ast
{
struct Member : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Member() override;
  Member(Executable const&) = delete;
  Member& operator=(Executable const&) = delete;
  Member(Executable&&) noexcept;
  Member& operator=(Executable&&) noexcept;

  Type& type() noexcept;

  ValueIdentifier& name() noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
