#pragma once

#include <memory>
#include <optional>

#include "ast/builder.hpp"
#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Type; }
namespace jackal::ast { struct ValueIdentifier; }
// clang-format on

namespace jackal::ast
{
struct Variable : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Variable() override;
  Variable(Variable const&) = delete;
  Variable& operator=(Variable const&) = delete;
  Variable(Variable&&) noexcept = delete;
  Variable& operator=(Variable&&) noexcept = delete;

  ValueIdentifier& name() noexcept;

  Type& type() noexcept;

  // TODO: determine if this is a correct construction
  std::optional<Ref<ValueIdentifier>> owner() noexcept;

  struct Builder : public AstBuilder
  {
    Builder& name(ValueIdentifier name) noexcept;
    Builder& owner(ValueIdentifier owner) noexcept;

    [[nodiscard]] Variable build() const noexcept;
  };

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
