﻿#pragma once
#include <doodle_global.h>
#include <core_global.h>

DOODLE_NAMESPACE_S
class toolkit {
 public:
  static void openPath(const doCore::fileSqlInfoPtr &info_ptr,
                       const bool &openEx);
  static void installUePath(const std::string &path);
 private:
};

DOODLE_NAMESPACE_E