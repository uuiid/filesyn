﻿#pragma once

#include <corelib/core_global.h>
#include <corelib/libWarp/BoostUuidWarp.h>
#include <corelib/core/Ue4Setting.h>
#include <corelib/Metadata/Project.h>

#include <cereal/cereal.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>

#include <magic_enum.hpp>
#include <boost/filesystem.hpp>

DOODLE_NAMESPACE_S

enum class Department {
  None_,
  Executive,
  Light,
  VFX,
  modle,
  rig,
  Anm,
  direct,
  paint
};

/*
 *全局静态设置类
 */

class CORE_API coreSet {
 public:
  static coreSet &getSet();

  DOODLE_DISABLE_COPY(coreSet)
  //初始化函数
  void init();
  void reInit();

  void findMaya();

  //获得运行程序目录
  static FSys::path program_location();
  static FSys::path program_location(const FSys::path &path);

  bool hasMaya() const noexcept;
  const FSys::path &MayaPath() const noexcept;
  void setMayaPath(const FSys::path &in_MayaPath) noexcept;

  // user设置
  [[nodiscard]] std::string getUser() const;
  [[nodiscard]] std::string getUser_en() const;
  void setUser(const std::string &value);

  //部门设置
  [[nodiscard]] std::string getDepartment() const;
  [[nodiscard]] const Department &getDepartmentEnum() const;
  void setDepartment(const std::string &value);

  //缓存路径
  [[nodiscard]] FSys::path getCacheRoot() const;
  [[nodiscard]] FSys::path getCacheRoot(const FSys::path &path) const;

  // doc路径
  [[nodiscard]] FSys::path getDoc() const;

  Ue4Setting &gettUe4Setting() const { return ue4_setting; };

  void writeDoodleLocalSet();

  static std::string toIpPath(const std::string &path);

  boost::uuids::uuid getUUID();

  bool hasProject();
  std::vector<ProjectPtr> getAllProjects() const;
  void installProject(const ProjectPtr &Project_);
  const ProjectPtr &Project_() const;
  void setProject_(const ProjectPtr &Project_);
  void setProject_(const Project *Project_);
  void deleteProject(const Project *Project_);
  int getProjectIndex() const;

 private:
  //私有化构造函数
  coreSet();
  //获得缓存磁盘路径
  void getCacheDiskPath();
  //获得本地的有限设置
  void getSetting();

  static std::string configFileName();
 private:
  boost::uuids::random_generator p_uuid_gen;

  //用户名称
  std::string user;
  //部门
  Department department;

  FSys::path cacheRoot;
  FSys::path doc;

  Ue4Setting &ue4_setting;

  std::vector<std::shared_ptr<Project>> p_project_list;
  std::shared_ptr<Project> p_project;
  FSys::path p_mayaPath;

  //这里是序列化的代码
  friend class cereal::access;
  template <class Archive>
  void save(Archive &ar, std::uint32_t const version) const;

  template <class Archive>
  void load(Archive &ar, std::uint32_t const version);
};

template <class Archive>
void coreSet::save(Archive &ar, std::uint32_t const version) const {
  ar(
      cereal::make_nvp("user", user),
      cereal::make_nvp("department", std::string{magic_enum::enum_name(department)}),
      cereal::make_nvp("ue4_setting", ue4_setting));
  ar(cereal::make_nvp("project", p_project_list),
     cereal::make_nvp("current project", p_project));
  ar(cereal::make_nvp("maya_Path", p_mayaPath));
}

template <class Archive>
void coreSet::load(Archive &ar, std::uint32_t const version) {
  std::string deps{};
  ar(
      user,
      deps,
      ue4_setting);

  this->department = magic_enum::enum_cast<doodle::Department>(deps).value_or(doodle::Department::VFX);

  if (version > 1) {
    ar(p_project_list, p_project);
  }
  if (version > 2)
    ar(p_mayaPath);
}

DOODLE_NAMESPACE_E

CEREAL_CLASS_VERSION(doodle::coreSet, 3);