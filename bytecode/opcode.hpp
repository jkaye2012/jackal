#pragma once

namespace jackal::bytecode
{
enum class opcode : unsigned char
{
  constant = 0,  // r_dest, val
  add = 1,       // r_dest, r_1, r_0
  store = 2,     // l_dest, r_0
  load = 3,      // r_dest, l_src
  print = 4      // r_src
};
}
