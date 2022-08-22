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

  Dependency(Type type, std::string include) : _type(type), _include(std::move(include)) {}

  Dependency(Type type, std::string include, std::string link)
      : _type(type), _include(std::move(include)), _link(std::move(link))
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
  std::string _include;
  std::optional<std::string> _link;
};
}  // namespace jackal::codegen::c
