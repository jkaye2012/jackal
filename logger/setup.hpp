#pragma once

#include <string>

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

namespace jackal::logger
{
/**
 * Configures the global logger.
 *
 * This function should be called as early as possible in the entrypoint of any executable
 * that uses Jackal libraries to ensure that logs are properly collected.
 */
void setup_default_logger()
{
  // TODO: create basic configuration framework and accept logging configuration here
  auto def_logger =
      spdlog::basic_logger_mt<spdlog::async_factory>("jackal_logger", "logs/jackal.log");

  spdlog::set_default_logger(def_logger);
}

/**
 * Ensures that the global logger is flushed.
 *
 * This should be called before an executable is allowed to complete to ensure that all
 * asynchronous logs are persisted to disk.
 */
void flush_default_logger() { spdlog::default_logger()->flush(); }

/**
 * An RAII wrapper around Jackal logging functionality.
 *
 * Creating a single Logger instance at the start of an executable is enough to configure
 * logging for all Jackal infrastructure. Only one Logger should be created per executable.
 */
struct Logger
{
  Logger() { setup_default_logger(); }
  ~Logger() { flush_default_logger(); }

  Logger(Logger const&) = delete;
  Logger& operator=(Logger const&) = delete;
  Logger(Logger&&) noexcept = delete;
  Logger& operator=(Logger&&) noexcept = delete;
};
}  // namespace jackal::logger
