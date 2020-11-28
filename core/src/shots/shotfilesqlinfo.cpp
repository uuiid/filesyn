﻿#include "shotfilesqlinfo.h"

#include <sqlpp11/mysql/mysql.h>
#include <sqlpp11/sqlpp11.h>
#include <src/core/coreDataManager.h>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <memory>

#include "Logger.h"
#include "src/coreOrm/basefile_sqlOrm.h"
#include "src/core/coreset.h"
#include "src/core/coresql.h"
#include "episodes.h"
#include "shot.h"
#include "shotClass.h"
#include "shottype.h"

CORE_NAMESPACE_S

shotFileSqlInfo::shotFileSqlInfo()
    : fileSqlInfo(),
      std::enable_shared_from_this<shotFileSqlInfo>(),
      p_shot_id(-1),
      p_eps_id(-1),
      p_shCla_id(-1),
      p_shTy_id(-1),
      p_ptr_eps(),
      p_ptr_shot(),
      p_ptr_shTy(),
      p_ptr_shcla() {}

void shotFileSqlInfo::select(const qint64& ID_) {
  doodle::Basefile tab{};

  auto db = coreSql::getCoreSql().getConnection();
  for (auto&& row : db->run(
           sqlpp::select(sqlpp::all_of(tab)).from(tab).where(tab.id == ID_))) {
    batchSetAttr(row);
  }
}

void shotFileSqlInfo::insert() {
  if (idP > 0) return;
  doodle::Basefile tab{};

  auto db = coreSql::getCoreSql().getConnection();
  auto install = sqlpp::dynamic_insert_into(*db, tab).dynamic_set(
      tab.file = fileP, tab.fileSuffixes = fileSuffixesP, tab.user = userP,
      tab.version = versionP, tab.FilePath_ = filepathP,
      tab.filestate = sqlpp::value_or_null(fileStateP),
      tab.projectId = coreSet::getSet().projectName().first);
  if (!infoP.empty())
    install.insert_list.add(tab.infor = strList_tojson(infoP));
  if (p_shot_id > 0) install.insert_list.add(tab.shotsId = p_shot_id);
  if (p_eps_id > 0) install.insert_list.add(tab.episodesId = p_eps_id);

  install.insert_list.add(tab.shotClassId =
                              shotClass::getCurrentClass()->getIdP());

  if (p_shTy_id > 0) install.insert_list.add(tab.shotTypeId = p_shTy_id);
  idP = db->insert(install);
  if (idP == 0) {
    DOODLE_LOG_WARN << fileStateP.c_str();
    throw std::runtime_error("");
  }
  coreDataManager::get().setShotInfoL(shared_from_this());
}

void shotFileSqlInfo::updateSQL() {
  if (idP < 0) return;
  doodle::Basefile tab{};

  auto db = coreSql::getCoreSql().getConnection();
  auto updata = sqlpp::update(tab);
  updata.set(tab.infor = strList_tojson(infoP), tab.filestate = fileStateP)
      .where(tab.id == idP);
  db->update(updata);
}

template <typename T>
void shotFileSqlInfo::batchSetAttr(T& row) {
  idP = row.id;
  fileP = row.file;
  fileSuffixesP = row.fileSuffixes;
  userP = row.user;
  versionP = row.version;
  filepathP = row.FilePath_;
  infoP = json_to_strList(row.infor);
  fileStateP = row.filestate;
  if (row.shotsId._is_valid) p_shot_id = row.shotsId;
  if (row.shotClassId._is_valid) {
    p_shCla_id = row.shotClassId;
    getShotType();
  }
  if (row.shotTypeId._is_valid) p_shTy_id = row.shotTypeId;
}

shotInfoPtrList shotFileSqlInfo::getAll(const episodesPtr& EP_) {
  doodle::Basefile tab{};
  shotInfoPtrList list{};

  auto db = coreSql::getCoreSql().getConnection();
  for (auto&& row : db->run(
           sqlpp::select(sqlpp::all_of(tab))
               .from(tab)
               .where(tab.episodesId == EP_->getIdP() and tab.shotsId.is_null())
               .order_by(tab.filetime.desc()))) {
    auto assInfo = std::make_shared<shotFileSqlInfo>();
    assInfo->batchSetAttr(row);
    assInfo->setEpisdes(EP_);
    list.push_back(assInfo);
  }
  coreDataManager::get().setShotInfoL(list);
  return list;
}

shotInfoPtrList shotFileSqlInfo::getAll(const shotPtr& sh_) {
  doodle::Basefile tab{};
  shotInfoPtrList list;

  auto db = coreSql::getCoreSql().getConnection();
  for (auto&& row : db->run(sqlpp::select(sqlpp::all_of(tab))
                                .from(tab)
                                .where(tab.shotsId == sh_->getIdP())
                                .order_by(tab.filetime.desc()))) {
    auto assInfo = std::make_shared<shotFileSqlInfo>();
    assInfo->batchSetAttr(row);
    assInfo->setShot(sh_);
    assInfo->exist(true);
    list.push_back(assInfo);
  }

  coreDataManager::get().setShotInfoL(list);
  return list;
}

shotInfoPtrList shotFileSqlInfo::getAll(const shotPtr& shot_ptr,
                                        const shotTypePtr& type_ptr) {
  doodle::Basefile tab{};
  shotInfoPtrList list;

  auto db = coreSql::getCoreSql().getConnection();
  auto& row = db->run(sqlpp::select(sqlpp::all_of(tab))
                          .from(tab)
                          .where(tab.shotsId == shot_ptr->getIdP() and
                                 tab.shotTypeId == type_ptr->getIdP())
                          .order_by(tab.filetime.desc())
                          .limit(1u))
                  .front();

  auto shotInfo = std::make_shared<shotFileSqlInfo>();
  if (row._is_valid) {
    //    shotInfo->batchSetAttr(row);

    shotInfo->idP = row.id;
    shotInfo->fileP = row.file.text;
    shotInfo->fileSuffixesP = row.fileSuffixes.text;
    shotInfo->userP = row.user.text;
    shotInfo->versionP = row.version;
    shotInfo->filepathP = row.FilePath_.text;
    shotInfo->infoP = shotInfo->json_to_strList(row.infor.text);
    shotInfo->fileStateP = row.filestate;
    if (row.shotsId._is_valid) shotInfo->p_shot_id = row.shotsId;
    if (row.shotClassId._is_valid) {
      shotInfo->p_shCla_id = row.shotClassId;
      shotInfo->getShotType();
    }
    if (row.shotTypeId._is_valid) shotInfo->p_shTy_id = row.shotTypeId;

    shotInfo->exist(true);
    list.push_back(shotInfo);
  } else {
    list.push_back(nullptr);
  }
  return list;
  //  return {};
}

shotInfoPtrList shotFileSqlInfo::getAll(const shotClassPtr& class_ptr) {
  doodle::Basefile tab{};
  shotInfoPtrList list;

  auto db = coreSql::getCoreSql().getConnection();
  for (auto&& row : db->run(sqlpp::select(sqlpp::all_of(tab))
                                .from(tab)
                                .where(tab.shotClassId == class_ptr->getIdP())
                                .order_by(tab.filetime.desc()))) {
    auto assInfo = std::make_shared<shotFileSqlInfo>();
    assInfo->batchSetAttr(row);
    assInfo->exist(true);
    list.push_back(assInfo);
  }
  coreDataManager::get().setShotInfoL(list);
  return list;
}

shotInfoPtrList shotFileSqlInfo::getAll(const shotTypePtr& type_ptr) {
  doodle::Basefile tab{};
  shotInfoPtrList list;

  auto db = coreSql::getCoreSql().getConnection();
  for (auto&& row : db->run(sqlpp::select(sqlpp::all_of(tab))
                                .from(tab)
                                .where(tab.shotTypeId == type_ptr->getIdP())
                                .order_by(tab.filetime.desc()))) {
    auto assInfo = std::make_shared<shotFileSqlInfo>();
    assInfo->batchSetAttr(row);
    assInfo->setShotType(type_ptr);
    assInfo->exist(true);
    list.push_back(assInfo);
  }
  coreDataManager::get().setShotInfoL(list);
  return list;
}

dpath shotFileSqlInfo::generatePath(const dstring& programFolder) {
  //第一次格式化添加根路径
  dpath path = coreSet::getSet().getShotRoot();

  //第二次格式化添加集数字符串
  path = path / getEpisdes()->getEpisdes_str();

  //第三次格式化添加镜头字符串
  auto shot = getShot();
  if (shot) path = path / shot->getShot_str();

  //第四次格式化添加程序文件夹
  path = path / programFolder;

  //第五次格式化添加部门文件夹
  path = path / coreSet::getSet().getDepartment();

  //第六次格式化添加类别文件夹
  path = path / getShotType()->getType();

  return path;
}

dpath shotFileSqlInfo::generatePath(const dstring& programFolder,
                                    const dstring& suffixes) {
  return generatePath(programFolder) / generateFileName(suffixes);
}

dpath shotFileSqlInfo::generatePath(const dstring& programFolder,
                                    const dstring& suffixes,
                                    const dstring& prefix) {
  return generatePath(programFolder) / generateFileName(suffixes, prefix);
}

dstring shotFileSqlInfo::generateFileName(const dstring& suffixes) {
  boost::format name("shot_%s_%s_%s_%s_v%04i_%s%s");

  //第一次 格式化添加 集数
  episodesPtr ep_ = getEpisdes();
  if (ep_)
    name % ep_->getEpisdes_str();
  else
    name % "";

  //第二次格式化添加 镜头号
  shotPtr sh_ = getShot();
  if (sh_)
    name % sh_->getShotAndAb_str();
  else
    name % "";

  //第三次格式化添加 fileclass
  if (getShotclass())
    name % coreSet::getSet().getDepartment();
  else
    name % "";

  //第四次格式化添加 shotType
  shotTypePtr ft_ = getShotType();
  if (ft_)
    name % ft_->getType();
  else
    name % "";

  name % versionP;
  name % coreSet::getSet().getUser_en();
  name % suffixes;

  return name.str();
}

dstring shotFileSqlInfo::generateFileName(const dstring& suffixes,
                                          const dstring& prefix) {
  boost::format name("%s_%s");
  name % prefix;
  name % generateFileName(suffixes);
  return name.str();
}

episodesPtr shotFileSqlInfo::getEpisdes() {
  if (p_ptr_eps) {
    return p_ptr_eps;
  } else if (p_eps_id >= 0) {
    episodesPtr p_ = std::make_shared<episodes>();
    p_->select(p_eps_id);
    this->setEpisdes(p_);
    return p_;
  }
  return nullptr;
}

void shotFileSqlInfo::setEpisdes(const episodesPtr& eps_) {
  if (!eps_) return;

  p_ptr_eps = eps_;
  p_eps_id = eps_->getIdP();
}

shotPtr shotFileSqlInfo::getShot() {
  if (p_ptr_shot != nullptr) {
    return p_ptr_shot;
  } else if (p_shot_id >= 0) {
    shotPtr p_ = std::make_shared<shot>();
    p_->select(p_shot_id);
    p_ptr_shot = p_;
    return p_;
  }
  return nullptr;
}

void shotFileSqlInfo::setShot(const shotPtr& shot_) {
  if (!shot_) return;
  p_ptr_shot = shot_;
  p_shot_id = shot_->getIdP();

  setEpisdes(shot_->getEpisodes());
}

shotClassPtr shotFileSqlInfo::getShotclass() {
  if (p_ptr_shcla)
    return p_ptr_shcla;
  else if (p_shCla_id >= 0) {
    for (const auto& item : coreDataManager::get().getShotClassL()) {
      if (item->getIdP() == p_shCla_id) {
        p_ptr_shcla = item;
        break;
      }
    }
    return p_ptr_shcla;
  }
  return nullptr;
}

void shotFileSqlInfo::setShotClass() {
  auto value = shotClass::getCurrentClass();
  if (!value) return;
  p_shCla_id = value->getIdP();
  p_ptr_shcla = value;
}

shotTypePtr shotFileSqlInfo::getShotType() {
  if (p_ptr_shTy)
    return p_ptr_shTy;
  else if (p_shTy_id >= 0) {
    for (const auto& item : coreDataManager::get().getShotTypeL()) {
      if (item->getIdP() == p_shTy_id) {
        p_ptr_shTy = item;
        break;
      }
    }
    return p_ptr_shTy;
  }
  return nullptr;
}
void shotFileSqlInfo::setShotType(const shotTypePtr& fileType_) {
  if (!fileType_) return;
  p_shTy_id = fileType_->getIdP();
  p_ptr_shTy = fileType_;

  versionP = getVersionMax() + 1;
  setShotClass();
}
bool shotFileSqlInfo::sort(const shotInfoPtr& t1, const shotInfoPtr& t2) {
  return (t1->getShotclass()->getClass_str() <
          t2->getShotclass()->getClass_str()) &&
         (t1->getShotType()->getType() < t2->getShotType()->getType());
}
int shotFileSqlInfo::getVersionMax() {
  for (const auto& info_l : coreDataManager::get().getShotInfoL()) {
    if ((getShotType() == info_l->getShotType()) &&
        (info_l->getShotclass() == shotClass::getCurrentClass()))
      return info_l->versionP;
  }
  return 0;
}

CORE_NAMESPACE_E