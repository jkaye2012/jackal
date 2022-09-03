#pragma once

#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

namespace jackal::codegen::c
{
struct Dependency
{
  enum class Type
  {
    System,
    Source
  };

  Dependency(Type type, std::string_view include) : _type(type), _include(include) {}

  Dependency(Type type, std::string_view include, std::string_view link)
      : _type(type), _include(include), _link(link)
  {
  }

  [[nodiscard]] Type type() const noexcept { return _type; }

  [[nodiscard]] std::string_view include() const noexcept { return _include; }

  [[nodiscard]] std::optional<std::string_view> link() const noexcept
  {
    return requires_link() ? std::optional<std::string_view>(*_link) : std::nullopt;
  }

  [[nodiscard]] bool requires_link() const noexcept { return _link.has_value(); }

  bool operator==(Dependency const& other) const noexcept
  {
    return type() == other.type() && include() == other.include() && link() == other.link();
  }

 private:
  Type _type;
  std::string_view _include;
  std::optional<std::string_view> _link;
};
}  // namespace jackal::codegen::c
