﻿/*
 * @Author: your name
 * @Date: 2020-09-02 09:59:06
 * @LastEditTime: 2020-12-01 13:50:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \Doodle\fileSystem\src\DfileSyntem.h
 */
#pragma once

#include <fileSystem/fileSystem_global.h>

DSYSTEM_S

class DSYSTEM_API DfileSyntem {
 public:
  ~DfileSyntem();
  DfileSyntem(const DfileSyntem &) = delete;
  DfileSyntem &operator=(const DfileSyntem &s) = delete;

  static DfileSyntem &get();
  ftpSessionPtr session() const;
  ftpSessionPtr session(const std::string &host,
                        int prot,
                        const std::string &name,
                        const std::string &password,
                        const dpath &netWork_disk);

  bool upload(const dpath &localFile, const dpath &remoteFile) noexcept;
  bool down(const dpath &localFile, const dpath &remoteFile) noexcept;
  bool exists(const dpath &remoteFile) const noexcept;
  std::unique_ptr<std::fstream> open(const dpath &remoteFile, std::ios_base::openmode mode) const noexcept;
  static bool copy(const dpath &sourePath, const dpath &trange_path, bool backup) noexcept;
  static bool removeDir(const dpath &path);

 private:
  DfileSyntem();
  std::string p_host_;
  int p_prot_;
  std::string p_name_;
  std::string p_password_;

  dpath p_netWork_disk_;
};

DSYSTEM_E