#pragma once

#include <memory>
#include <variant>
#include <vector>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Member; }
namespace jackal::ast { struct Type; }
// clang-format on

namespace jackal::ast
{
struct Data : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Data() override;
  Data(Data const&) = delete;
  Data& operator=(Data const&) = delete;
  Data(Data&&) noexcept;
  Data& operator=(Data&&) noexcept;

  Type& type() noexcept;

  std::vector<Member>& members() noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
