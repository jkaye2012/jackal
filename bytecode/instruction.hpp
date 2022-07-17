#pragma once

#include "bytecode/opcode.hpp"

namespace jackal::bytecode
{
/**
 * A binary encoding of a single virtual machine instruction.
 *
 * The Jackal VM executes these instructions sequentially per thread.
 *
 * Instructions are variable-length depending upon required operands.
 */
struct instruction
{
  opcode opcode;
};
}  // namespace jackal::bytecode
