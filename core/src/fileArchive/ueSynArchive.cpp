﻿//
// Created by teXiao on 2020/11/11.
//

#include "ueSynArchive.h"

#include <src/DfileSyntem.h>
#include <src/assets/assfilesqlinfo.h>
#include <src/core/coreDataManager.h>
#include <src/core/coreset.h>
#include <src/shots/episodes.h>
#include <src/exeWrap/freeSynWrap.h>
#include <src/ftpsession.h>
#include <src/shots/shot.h>
#include <src/sysData/synData.h>

#include <boost/format.hpp>
CORE_NAMESPACE_S
ueSynArchive::ueSynArchive()
    : fileArchive(), p_syn(std::make_shared<freeSynWrap>()), synpart() {}
void ueSynArchive::insertDB() {}
void ueSynArchive::_generateFilePath() {}
dpath ueSynArchive::syn(const shotPtr &shot_) {
  auto &set = coreSet::getSet();
  boost::format str("/03_Workflow/Assets/%s/backup");
  str % set.getDepartment();
  synpart = set.getSynDir();
  if (synpart.empty()) {
    return {};
  }

  //  boost::format eps(DOODLE_EPFORMAT);
  //  eps % set.getSyneps();
  dstring shotFstr = "*\\VFX\\*";
  if (shot_) {
    boost::format shotFlliter(R"(*c%04i\Checkpoint\VFX\*)");
    shotFstr = (shotFlliter % shot_->getShot()).str();
  }
  for (auto &item : synpart) {
    item.local = set.getSynPathLocale() / set.projectName().second /
                 item.local / DOODLE_CONTENT / "shot";
    item.server = set.getAssRoot() / set.getDepartment() / item.server /
                  DOODLE_CONTENT / "shot";
  }

  p_syn->addSynFile(synpart);
  p_syn->setVersioningFolder(freeSynWrap::syn_set::twoWay, str.str());
  if (set.getDepartment() == "VFX") {
    //设置同步方式
    p_syn->addInclude({shotFstr});

  } else if (set.getDepartment() == "Light") {
    //同步light镜头
    for (int i = 0; i < synpart.size(); ++i) {
      p_syn->addSubSynchronize(i, freeSynWrap::syn_set::twoWay, str.str());
      p_syn->addSubIncludeExclude(i, {"*"}, {shotFstr});
    }

    //下载vfx镜头
    auto syn_part_vfx = set.getSynDir();
    for (auto &item : syn_part_vfx) {
      item.local = set.getSynPathLocale() / set.projectName().second /
                   item.local / DOODLE_CONTENT / "shot";
      item.server =
          set.getAssRoot() / "VFX" / item.server / DOODLE_CONTENT / "shot";
    }
    p_syn->addSynFile(syn_part_vfx);
    for (size_t i = syn_part_vfx.size();
         i < syn_part_vfx.size() + synpart.size(); ++i) {
      p_syn->addSubSynchronize(i, freeSynWrap::syn_set::down, str.str());
      p_syn->addSubIncludeExclude(i, {shotFstr}, {});
    }
  }
  p_syn->run();

  return {};
}

bool ueSynArchive::update() {
  syn(nullptr);
  return true;
}
bool ueSynArchive::makeDir(const episodesPtr &episodes_ptr) {
  auto synClass = synData::getAll(episodes_ptr);
  auto assClass = coreDataManager::get().getAssClassPtr();
  //复制文件的来源
  auto soure_ptr = coreDataManager::get().getAssInfoPtr();

  //验证各种指针
  if (!episodes_ptr) return false;
  if (synClass->isNULL()) {
    synClass->setEpisodes(episodes_ptr);
    synClass->insert();
  }
  if (!soure_ptr) return false;

  auto create_path = synClass->push_back(assClass);

  synClass->updateSQL();

  auto &set = coreSet::getSet();

  dstringList list;
  //复制灯光需要的文件夹
  auto server = set.getPrjectRoot() / set.getAssRoot() / set.getDepartment() /
                *create_path;
  auto copy_path_list = soure_ptr->getFileList();
  if (copy_path_list.size() == 1) {
    auto path = copy_path_list.front();
    doSystem::DfileSyntem::copy(set.getPrjectRoot() / path,
                                server / path.filename());
    doSystem::DfileSyntem::copy(
        set.getPrjectRoot() / path.parent_path() / DOODLE_CONTENT,
        server / DOODLE_CONTENT);
  } else if (copy_path_list.size() == 2) {
    for (auto &copy_path : copy_path_list) {
      doSystem::DfileSyntem::copy(set.getPrjectRoot() / copy_path,
                                  server / copy_path.filename());
    }
  }

  for (auto &&path : soure_ptr->getFileList()) {
  }
  //创建灯光需要的文件夹
  boost::format shot(DOODLE_SHFORMAT);
  dstringList listcreates{"Checkpoint", "Light", "Ren"};
  dstringList listDep{"Light", "VFX"};
  auto session = doSystem::DfileSyntem::getFTP().session(
      set.getIpFtp(), 21, set.getProjectname() + set.getUser_en(),
      set.getUser_en());
  server = set.getAssRoot() / set.getDepartment() / *create_path /
           DOODLE_CONTENT / "shot" / episodes_ptr->getEpisdes_str();
  session->createDir(server.generic_string());

  for (int kI = 0; kI < 120; ++kI) {
    auto ks1 = server / (shot % kI).str();
    session->createDir({ks1.generic_string()}, false);

    for (const auto &listcreate : listcreates) {
      auto ks3 = ks1 / listcreate;
      session->createDir({ks3.generic_string()}, false);

      if (listcreate == *listcreates.begin()) {
        for (const auto &list_dep : listDep) {
          auto ks2 = ks3 / list_dep;
          session->createDir({ks2.generic_string()}, false);
        }
      }
    }
  }
  return true;
}
CORE_NAMESPACE_E