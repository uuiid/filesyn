﻿/*
 * @Author: your name
 * @Date: 2020-10-21 13:37:17
 * @LastEditTime: 2020-12-14 13:44:52
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \Doodle\core\src\archiveFactory.h
 */
//
// Created by teXiao on 2020/10/21.
//
#pragma once

#include <corelib/core_global.h>

DOODLE_NAMESPACE_S
class CORE_API archiveFactory {
 public:
  static fileArchivePtr crearte(const QString& suffix);
};

DOODLE_NAMESPACE_E