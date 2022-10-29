#pragma once

#include <cstddef>
#include <memory>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Environment; }
// clang-format on

namespace jackal::ast
{
struct Arguments : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Arguments() override;
  Arguments(Arguments const&) = delete;
  Arguments& operator=(Arguments const&) = delete;
  Arguments(Arguments&&) noexcept;
  Arguments& operator=(Arguments&&) noexcept;

  [[nodiscard]] size_t length() const noexcept;

  Environment& bindings() noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
