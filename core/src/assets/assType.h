﻿//
// Created by teXiao on 2020/11/6.
//
#pragma once

#include "core_global.h"

#include <src/core/coresqldata.h>
CORE_NAMESPACE_S

class CORE_API assType
    : public coresqldata,
      public std::enable_shared_from_this<assType> {
 public:
  enum e_type{
    UE4,
    scenes,
    rig,
    scenes_low,
    sourceimages,
    screenshot,
  };
 public:
  explicit assType();
  void select(const int64_t &ID_);
  void insert() override;
  void updateSQL() override;
  void deleteSQL() override;

  static assTypePtrList getAll();
  static assTypePtr findType(const e_type &typeName, bool autoInstall);
  static bool sortType(const assTypePtr &t1,const assTypePtr &t2);
 private:
  static assTypePtr findType(const std::string& typeName);
  std::string s_type;
 public:
  [[nodiscard]] const std::string &getType() const;
  [[nodiscard]] const QString getTypeQ() const;
  void setType(const std::string &string);
  void setType(const QString &string);
 private:
  int64_t p_ass_class_id;

};

inline const QString assType::getTypeQ() const {
  return QString::fromStdString(getType());
}
inline void assType::setType(const QString &string) {
  setType(string.toStdString());
}
CORE_NAMESPACE_E