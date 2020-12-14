﻿/*
 * @Author: your name
 * @Date: 2020-11-06 09:22:09
 * @LastEditTime: 2020-12-14 13:28:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \Doodle\core\src\assets\assdepartment.cpp
 */
//
// Created by teXiao on 2020/11/6.
//

#include "assdepartment.h"
#include <src/core/coreset.h>
#include <src/core/coresql.h>

#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>
#include <src/coreOrm/assdepartment_sqlOrm.h>

#include <stdexcept>
#include <memory>
#include <src/core/coreDataManager.h>
#include <QString>

//反射使用
#include <rttr/registration>
CORE_NAMESPACE_S

RTTR_REGISTRATION {
  rttr::registration::class_<assdepartment>(DOCORE_RTTE_CLASS(assdepartment))
      .constructor<>()(rttr::policy::ctor::as_std_shared_ptr);
}

assdepartment::assdepartment()
    : coresqldata(),
      std::enable_shared_from_this<assdepartment>(),
      i_prjID(-1),
      s_assDep("character") {
  i_prjID = coreSet::getSet().projectName().first;
}

void assdepartment::insert() {
  i_prjID = coreSet::getSet().projectName().first;

  auto db = coreSql::getCoreSql().getConnection();
  doodle::Assdepartment table{};

  auto insert = sqlpp::insert_into(table).columns(table.projectId, table.assDep);
  insert.values.add(table.projectId = i_prjID,
                    table.assDep = s_assDep);
  idP = db->insert(insert);
  if (idP == 0) {
    throw std::runtime_error("not install assDep");
  }
  coreDataManager::get().setAssDepL(shared_from_this());
}
void assdepartment::updateSQL() {
}
void assdepartment::deleteSQL() {
}
const std::string &assdepartment::getAssDep() const {
  return s_assDep;
}
void assdepartment::setAssDep(const std::string &s_ass_dep) {
  s_assDep = s_ass_dep;
}
assDepPtrList assdepartment::getAll() {
  assDepPtrList list;
  doodle::Assdepartment table{};
  auto db = coreSql::getCoreSql().getConnection();
  for (auto &&row : db->run(
           sqlpp::select(sqlpp::all_of(table))
               .from(table)
               .where(table.projectId == coreSet::getSet().projectName().first))) {
    auto assdep = std::make_shared<assdepartment>();
    assdep->s_assDep = row.assDep.value();
    assdep->idP = row.id.value();
    assdep->i_prjID = row.projectId.value();
    list.push_back(assdep);
  }
  coreDataManager::get().setAssDepL(list);
  return list;
}
void assdepartment::select(const int64_t &ID_) {
}
const QString assdepartment::getAssDepQ() const {
  return QString::fromStdString(getAssDep());
}

CORE_NAMESPACE_E