#pragma once

#include <memory>
#include <variant>

#include "ast/builder.hpp"
#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Data; }
namespace jackal::ast { struct Function; }
// clang-format on

namespace jackal::ast
{
struct Form : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Form() override;
  Form(Form const&) = delete;
  Form& operator=(Form const&) = delete;
  Form(Form&&) noexcept;
  Form& operator=(Form&&) noexcept;

  std::variant<Ref<Data>, Ref<Function>>& definition() noexcept;

  struct Builder : public ast::AstBuilder
  {
    Builder& data(Data data) noexcept;
    Builder& function(Function fn) noexcept;

    [[nodiscard]] Form build() const noexcept;
  };

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
