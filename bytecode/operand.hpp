#pragma once

#include <cstddef>
#include <cstdint>
#include <variant>

namespace jackal::bytecode
{
enum class operand_type : std::size_t
{
  constant = 0,
  reg = 1,
  local = 2
};

static constexpr std::size_t constant_index = static_cast<std::size_t>(operand_type::constant);
static constexpr std::size_t register_index = static_cast<std::size_t>(operand_type::reg);
static constexpr std::size_t local_index = static_cast<std::size_t>(operand_type::local);

/**
 * Represents all operands available to the Jackal bytecode virtual machine.
 *
 * All instructions that require input expect to receive a specific operand type.
 */
struct operand
{
  static constexpr operand from_constant(int64_t val) noexcept
  {
    return operand(storage{std::in_place_index<constant_index>, val});
  }

  static constexpr operand from_register(uint64_t reg) noexcept
  {
    return operand(storage{std::in_place_index<register_index>, reg});
  }

  static constexpr operand from_local(uint64_t loc) noexcept
  {
    return operand(storage{std::in_place_index<local_index>, loc});
  }

  constexpr operand_type type() const noexcept
  {
    return static_cast<operand_type>(_storage.index());
  }

  constexpr int64_t as_constant() const noexcept { return std::get<constant_index>(_storage); }

  constexpr uint64_t as_register() const noexcept { return std::get<register_index>(_storage); }

  constexpr uint64_t as_local() const noexcept { return std::get<local_index>(_storage); }

 private:
  using storage = std::variant<int64_t, uint64_t, uint64_t>;
  storage _storage;

  explicit constexpr operand(storage st) noexcept : _storage(std::move(st)) {}
};
}  // namespace jackal::bytecode
