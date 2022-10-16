#pragma once

#include <cstddef>
#include <memory>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Type; }
// clang-format on

namespace jackal::ast
{
struct Parameters : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Parameters() override;
  Parameters(Parameters const&) = delete;
  Parameters& operator=(Parameters const&) = delete;
  Parameters(Parameters&&) noexcept = delete;
  Parameters& operator=(Parameters&&) noexcept = delete;

  [[nodiscard]] size_t length() const noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
