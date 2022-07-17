#pragma once

#include <cstdint>
namespace jackal::bytecode
{
/**
 * Represents a single register that can be used by the Jackal bytecode VM.
 *
 * For phase 1 implementation, registers can hold only 64-bit signed integers.
 */
struct reg
{
  explicit constexpr reg(int64_t value) noexcept : _value(value) {}

  constexpr int64_t value() const noexcept { return _value; }

  constexpr void set_value(int64_t value) noexcept { _value = value; }

 private:
  int64_t _value;
};
}  // namespace jackal::bytecode
