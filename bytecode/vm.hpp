#pragma once

#include <vector>

#include "bytecode/reg.hpp"

namespace jackal::bytecode
{
// Phase 1 requires:
// * Registers
// * Locals
//
// Everything can be typed as 64-bit signed integers.
// No functions yet, so all memory can be globally scoped.
struct vm
{
 private:
  std::vector<reg> _registers;
  // For phase 1, registers and locals have identical structure.
  // Beyond "everything is long", this will require different storage,
  // but for now this is just convenient re-use.
  std::vector<reg> _locals;
};
}  // namespace jackal::bytecode
