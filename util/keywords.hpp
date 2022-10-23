#pragma once

#include <array>
#include <string_view>

namespace jackal::util::keyword
{
constexpr auto Let = "let";
constexpr auto Fn = "fn";
constexpr auto Data = "data";
constexpr auto Satisfy = "satisfy";
constexpr auto Concept = "concept";
constexpr auto Interpret = "interpret";
constexpr auto Effect = "effect";

constexpr std::array<std::string_view, 7> AllKeywords{Let,     Fn,        Data,  Satisfy,
                                                      Concept, Interpret, Effect};
}  // namespace jackal::util::keyword
