﻿/*
 * @Author: your name
 * @Date: 2020-09-10 09:56:04
 * @LastEditTime: 2020-11-26 20:38:26
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \Doodle\core\src\filesqlinfo.h
 */
#pragma once

#include "core_global.h"
#include "src/core/coresqldata.h"

#include <QVector>
#include <QFileInfo>
#include <QSharedPointer>
#include <QWeakPointer>

CORE_NAMESPACE_S

class CORE_API fileSqlInfo : public coresqldata {
 public:
  //属性设置和查询
  fileSqlInfo();
  [[nodiscard]] dpathList getFileList() const;
  virtual void setFileList(const dpathList &filelist);
  virtual void setFileList(const dstringList &filelist);
  [[nodiscard]] int getVersionP() const;
  void setVersionP(const int64_t &value);

  [[nodiscard]] dstringList getInfoP() const;
  void setInfoP(const dstring &value);

  [[nodiscard]] dstring getFileStateP() const;
  void setFileStateP(const dstring &value);

  [[nodiscard]] dstring getSuffixes() const;
  [[nodiscard]] QString getSuffixesQ() const;

  [[nodiscard]] dstring getUser() const;
  [[nodiscard]] QString getUserQ() const;
  virtual dpath generatePath(const std::string &programFodler) = 0;
  virtual dpath generatePath(const dstring &programFolder,
                             const dstring &suffixes) = 0;
  virtual dpath generatePath(const dstring &programFolder,
                             const dstring &suffixes,
                             const dstring &prefix) = 0;
  virtual dstring generateFileName(const dstring &suffixes) = 0;
  virtual dstring generateFileName(const dstring &suffixes,
                                   const dstring &prefix) = 0;
  virtual void deleteSQL() override;

  virtual bool exist(bool refresh);

 protected:
  //属性包装

  std::string fileP;
  std::string fileSuffixesP;
  std::string userP;
  int versionP;
  std::string filepathP;
  dstringList infoP;
  std::string fileStateP;
  bool p_b_exist;

 protected:
  [[nodiscard]] dstringList json_to_strList(const dstring &json_str) const;
  [[nodiscard]] dstring strList_tojson(const dstringList &str_list) const;
};
inline QString fileSqlInfo::getUserQ() const {
  return QString::fromStdString(getUser());
}
inline QString fileSqlInfo::getSuffixesQ() const {
  return QString::fromStdString(getSuffixes());
}
CORE_NAMESPACE_E