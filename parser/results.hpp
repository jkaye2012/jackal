/// @file Forward declarations and type aliases used within the parser.
///
/// Consolidating these aliases simplifies the concrete parsers implementations, allowing developers
/// to focus on the parsing logic without requiring significant boilerplate.
#pragma once

// clang-format off
namespace jackal::ast { struct Executable; }
namespace jackal::ast { struct Expression; }
namespace jackal::ast { struct Function; }
namespace jackal::ast { struct Number; }
namespace jackal::ast { struct Primitive; }
namespace jackal::ast { struct ValueV1; }
namespace jackal::ast { struct Variable; }
namespace jackal::lexer { struct Token; }
namespace jackal::parser { struct ParseError; }
namespace jackal::util { template<typename, typename> struct Result; }
// clang-format on

namespace jackal::parser
{
using ExecutableResult = jackal::util::Result<jackal::ast::Executable, jackal::parser::ParseError>;
using ExpressionResult = jackal::util::Result<jackal::ast::Expression, jackal::parser::ParseError>;
using FunctionResult = jackal::util::Result<jackal::ast::Function, jackal::parser::ParseError>;
using NumberResult = jackal::util::Result<jackal::ast::Number, jackal::parser::ParseError>;
using PrimitiveResult = jackal::util::Result<jackal::ast::Primitive, jackal::parser::ParseError>;
using ValueResult = jackal::util::Result<jackal::ast::ValueV1, jackal::parser::ParseError>;
using VariableResult = jackal::util::Result<jackal::ast::Variable, jackal::parser::ParseError>;
using TokenResult = jackal::util::Result<jackal::lexer::Token, jackal::parser::ParseError>;
}  // namespace jackal::parser
