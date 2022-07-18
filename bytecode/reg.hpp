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
  explicit constexpr reg(uint64_t num) noexcept : _num(num), _value(0) {}
  constexpr reg(uint64_t num, int64_t value) noexcept : _num(num), _value(value) {}

  constexpr uint64_t num() const noexcept { return _num; }

  constexpr int64_t value() const noexcept { return _value; }

  constexpr void set_value(int64_t value) noexcept { _value = value; }

 private:
  uint64_t _num;
  int64_t _value;
};
}  // namespace jackal::bytecode
