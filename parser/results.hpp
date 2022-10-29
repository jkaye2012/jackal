/// @file Forward declarations and type aliases used within the parser.
///
/// Consolidating these aliases simplifies the concrete parsers implementations, allowing developers
/// to focus on the parsing logic without requiring significant boilerplate.
#pragma once

// clang-format off
namespace jackal::parser { struct ParseError; }
namespace jackal::util { template<typename, typename> struct Result; }
// clang-format on

namespace jackal::parser
{
template <typename AstNode>
using ParseResult = jackal::util::Result<AstNode, jackal::parser::ParseError>;
}  // namespace jackal::parser
