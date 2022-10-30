#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "ast/builder.hpp"
#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Type; }
namespace jackal::ast { struct ValueIdentifier; }
// clang-format on

namespace jackal::ast
{
struct Argument : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  Argument(ValueIdentifier name, Type type) noexcept;

  ~Argument() override;
  Argument(Argument const&) = delete;
  Argument& operator=(Argument const&) = delete;
  Argument(Argument&&) noexcept;
  Argument& operator=(Argument&&) noexcept;

  ValueIdentifier& name() noexcept;

  Type& type() noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};

struct Arguments : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Arguments() override;
  Arguments(Arguments const&) = delete;
  Arguments& operator=(Arguments const&) = delete;
  Arguments(Arguments&&) noexcept;
  Arguments& operator=(Arguments&&) noexcept;

  [[nodiscard]] size_t length() const noexcept;
  [[nodiscard]] bool empty() const noexcept;

  // TODO: avoid dynamic allocation here?
  std::vector<Ref<Argument>> args() noexcept;

  struct Builder : public AstBuilder
  {
    Builder& append_arg(Argument arg) noexcept;

    [[nodiscard]] Arguments build() const noexcept;
  };

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
