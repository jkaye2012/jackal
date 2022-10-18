#pragma once

#include <memory>
#include <variant>

#include "ast/builder.hpp"
#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Number; }
namespace jackal::ast { struct Type; }
// clang-format on

namespace jackal::ast
{
struct Primitive : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Primitive() override;
  Primitive(Primitive const&) = delete;
  Primitive& operator=(Primitive const&) = delete;
  Primitive(Primitive&&) noexcept = default;
  Primitive& operator=(Primitive&&) noexcept = default;

  Type& type() noexcept;

  std::variant<Ref<Number>>& value() noexcept;

  struct Builder : public AstBuilder
  {
    Builder& number(Number num) noexcept;

    [[nodiscard]] Primitive build() const noexcept;
  };

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
