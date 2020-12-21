﻿#include "shotClass.h"

#include <Logger.h>
#include <src/core/coreset.h>
#include <src/core/coresql.h>
#include <src/shots/episodes.h>
#include <src/shots/shot.h>

#include <sqlpp11/mysql/mysql.h>
#include <sqlpp11/sqlpp11.h>
#include <src/coreOrm/shotclass_sqlOrm.h>

#include <iostream>
#include <magic_enum.hpp>
#include <memory>

//反射使用
#include <rttr/registration>
CORE_NAMESPACE_S

RTTR_REGISTRATION {
  rttr::registration::class_<shotClass>(DOCORE_RTTE_CLASS(shotClass))
      .constructor<>()(rttr::policy::ctor::as_std_shared_ptr);
}
DOODLE_INSRANCE_CPP(shotClass);
shotClass::shotClass()
    : coresqldata(),
      std::enable_shared_from_this<shotClass>(),
      p_fileclass(e_fileclass::_) {}

shotClass::~shotClass() {
  DOODLE_LOG_DEBUG(p_instance.size());
  if (isInsert())
    p_instance.erase(idP);
}

void shotClass::select(const qint64 &ID_) {
  doodle::Shotclass table{};
  auto db = coreSql::getCoreSql().getConnection();
  for (auto &&row : db->run(sqlpp::select(sqlpp::all_of(table))
                                .from(table)
                                .where(table.id == ID_))) {
    idP = row.id;
    setclass(row.shotClass);
  }
  p_instance.insert({idP, this});
}

void shotClass::insert() {
  if (idP > 0) return;

  doodle::Shotclass table{};
  auto db      = coreSql::getCoreSql().getConnection();
  auto install = sqlpp::insert_into(table).set(
      table.shotClass = getClass_str(),
      table.projectId = coreSet::getSet().projectName().first);
  idP = db->insert(install);
  if (idP == 0) {
    DOODLE_LOG_WARN("无法插入shot type" << getClass_str().c_str());
    throw std::runtime_error("not install shot");
  }
  p_instance.insert({idP, this});
}

void shotClass::updateSQL() {
  if (idP < 0) return;
  doodle::Shotclass table{};
  auto db = coreSql::getCoreSql().getConnection();
  db->update(sqlpp::update(table)
                 .set(table.shotClass = getClass_str())
                 .where(table.id == idP));
}

void shotClass::deleteSQL() {
  doodle::Shotclass table{};
  auto db = coreSql::getCoreSql().getConnection();
  db->remove(sqlpp::remove_from(table).where(table.id == idP));
}
template <typename T>
void shotClass::batchSetAttr(T &row) {
  idP = row.id;
  setclass(row.shotClass);
}

shotClassPtrList shotClass::getAll() {
  shotClassPtrList list{};

  doodle::Shotclass table{};
  auto db = coreSql::getCoreSql().getConnection();
  for (auto &&row : db->run(
           sqlpp::select(sqlpp::all_of(table))
               .from(table)
               .where(table.projectId == coreSet::getSet().projectName().first)
               .order_by(table.shotClass.desc()))) {
    auto item = std::make_shared<shotClass>();
    item->batchSetAttr(row);
    list.push_back(item);
    p_instance.insert({item->idP, item.get()});
  }
  DOODLE_LOG_DEBUG("loaded fileClasses " << list.size());
  return list;
}

shotClassPtr shotClass::getCurrentClass() {
  shotClassPtr ptr = nullptr;
  for (auto &item : p_instance) {
    if (item.second->getClass_str() == coreSet::getSet().getDepartment()) {
      ptr = item.second->shared_from_this();
      break;
    }
  }
  if (!ptr) {
    DOODLE_LOG_ERROR("find not shot class " << coreSet::getSet().getDepartment())
    throw std::runtime_error("");
  }

  return ptr;
}

dstring shotClass::getClass_str() const {
  std::string str(magic_enum::enum_name(p_fileclass));
  return str;
}

shotClass::e_fileclass shotClass::getClass() const { return p_fileclass; }

void shotClass::setclass(const e_fileclass &value) { p_fileclass = value; }

void shotClass::setclass(const dstring &value) {
  auto tmp_fc = magic_enum::enum_cast<e_fileclass>(value);
  if (tmp_fc.has_value()) {
    p_fileclass = tmp_fc.value();
  } else {
    throw std::runtime_error("not file class in enum");
  }
}
const std::map<int64_t, shotClass *> &shotClass::Instances() {
  return p_instance;
}
CORE_NAMESPACE_E
