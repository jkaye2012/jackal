#pragma once

#include <memory>
#include <vector>

#include "ast/builder.hpp"
#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Type; }
// clang-format on

namespace jackal::ast
{
struct Context : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { return visitor.visit(*this); }

  Context() noexcept;

  ~Context() override;
  Context(Context const&) = delete;
  Context& operator=(Context const&) = delete;
  Context(Context&&) noexcept;
  Context& operator=(Context&&) noexcept;

  std::vector<Type>& types() noexcept;

  [[nodiscard]] bool empty() const noexcept;

  struct Builder : public AstBuilder
  {
    Builder& append_type(Type type) noexcept;

    [[nodiscard]] Context build() const noexcept;
  };

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
