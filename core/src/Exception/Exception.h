#pragma once

#include "core_global.h"
#include <stdexcept>

DOODLE_NAMESPACE_S

//----------------------------------------------------------------
class CORE_API not_file_error : public std::runtime_error {
 public:
  not_file_error(const std::string &err) : std::runtime_error(err){};
  virtual const char *what() const noexcept override;
};

//----------------------------------------------------------------
class CORE_API copy_error : public std::runtime_error {
 public:
  copy_error(const std::string &err) : std::runtime_error(err){};
  virtual const char *what() const noexcept override;
};

//----------------------------------------------------------------
class CORE_API upload_error : public std::runtime_error {
 public:
  upload_error(const std::string &err) : std::runtime_error(err){};
  virtual const char *what() const noexcept override;
};

//----------------------------------------------------------------
class CORE_API insert_error_info : public std::runtime_error {
 public:
  insert_error_info(const std::string &err) : std::runtime_error(err){};
  virtual const char *what() const noexcept override;
};

class CORE_API find_error_info : public std::runtime_error {
 public:
  find_error_info(const std::string &err) : std::runtime_error(err){};
  virtual const char *what() const noexcept override;
};

class nullptr_error : public std::runtime_error {
 public:
  nullptr_error(const std::string &err) : std::runtime_error(err){};
  virtual const char *what() const noexcept override;
};

DOODLE_NAMESPACE_E