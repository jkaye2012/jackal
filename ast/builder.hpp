#pragma once

namespace jackal::ast
{
// TODO: should this take a template param and enforce the presence of a T build() const noexcept?
struct AstBuilder
{
  [[nodiscard]] constexpr bool was_modified() const noexcept { return _modified; }

 protected:
  constexpr void modified() noexcept { _modified = true; }

 private:
  bool _modified = false;
};
}  // namespace jackal::ast
