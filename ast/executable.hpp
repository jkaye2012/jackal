#pragma once

#include <memory>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Function; }
// clang-format on

namespace jackal::ast
{
struct Executable : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Executable() override;
  Executable(Executable const&) = delete;
  Executable& operator=(Executable const&) = delete;
  Executable(Executable&&) noexcept = delete;
  Executable& operator=(Executable&&) noexcept = delete;

  Function& main() noexcept;

  // TODO: do we need an idea of (top-level) Form?

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
