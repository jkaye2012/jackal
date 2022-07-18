#pragma once

#include <array>
#include <cstddef>
#include <optional>
#include <type_traits>

#include "bytecode/opcode.hpp"
#include "bytecode/operand.hpp"
#include "bytecode/reg.hpp"

namespace jackal::bytecode
{
template <std::size_t N>
using operands = std::array<operand, N>;

template <std::size_t N, bool HasOutput>
struct instruction_storage;

template <std::size_t N>
struct instruction_storage<N, true>
{
  opcode opcode;
  reg output;
  operands<N> operands;
};

template <std::size_t N>
struct instruction_storage<N, false>
{
  opcode opcode;
  operands<N> operands;
};

/**
 * A binary encoding of a single virtual machine instruction.
 *
 * The Jackal VM executes these instructions sequentially per thread.
 *
 * Instructions are variable-length depending upon required operands.
 *
 * @tparam N The number of operands accepted by the instruction
 */
template <std::size_t N, bool HasOutput>
struct instruction
{
  explicit constexpr instruction(opcode op) noexcept : instruction(op, {}) {}

  constexpr instruction(opcode op, operands<N> operands) noexcept
      : _storage{op, std::move(operands)}
  {
  }

  constexpr instruction(opcode op, operands<N> operands, reg output) noexcept
      : _storage{op, std::move(output), std::move(operands)}
  {
  }

  constexpr opcode get_opcode() const noexcept { return _storage.opcode; }

  template <std::size_t M>
  constexpr operand get_operand() const noexcept
  {
    if constexpr (M < N)
    {
      return _storage.operands[M];
    }
  }

  constexpr reg get_output() const noexcept { return _storage.output.value(); }

 private:
  instruction_storage<N, HasOutput> _storage;
};
}  // namespace jackal::bytecode
