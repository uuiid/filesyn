﻿#include "ftpsession.h"

#include "Logger.h"
#include <fstream>
#include <QUrl>
#include <QFile>
#include <QDir>

#include <QDirIterator>

FTPSPACE_S
ftpSession::ftpSession() {
  ptrUrl = QSharedPointer<QUrl>(new QUrl());
  outfile = QSharedPointer<QFile>(new QFile());
  inputfile = QSharedPointer<QFile>(new QFile());

  curlSession = curl_easy_init();
}

ftpSession::~ftpSession() {
  curl_easy_cleanup(curlSession);
}

void ftpSession::setInfo(const QString &host, int prot, const QString &name, const QString &password) {
  ptrUrl->setScheme("ftp");
  ptrUrl->setHost(host);
  ptrUrl->setPort(prot);
  if (!name.isEmpty()) {
    ptrUrl->setUserName(name);
  } else {
    ptrUrl->setUserName("anonymous");
  }
  if (!password.isEmpty()) {
    ptrUrl->setPassword(password);
  } else {
    ptrUrl->setPassword("");
  }
}

bool ftpSession::down(const QString &localFile, const QString &remoteFile) {
  //打开文件
  outfile->setFileName(localFile);
  if (!outfile->open(QIODevice::WriteOnly)) {
    throw std::runtime_error("not open file");
  }
  //设置url路径
  ptrUrl->setPath(remoteFile);
  curl_easy_reset(curlSession);

  //创建下载设置
  curl_easy_setopt(curlSession, CURLOPT_URL, ptrUrl->toString().toStdString().c_str());
  curl_easy_setopt(curlSession, CURLOPT_WRITEFUNCTION, &ftpSession::writeFileCallbask);
  curl_easy_setopt(curlSession, CURLOPT_WRITEDATA, this->outfile.get());

  //检查下载是否成功
  CURLcode err = perform();
  outfile->close();
  if (err != CURLE_OK) {

    DOODLE_LOG_WARN << err << curl_easy_strerror(err) << outfile->fileName();
    outfile->remove();
    return false;
  }
  emit finished();
  return true;
}

bool ftpSession::upload(const QString &localFile, const QString &remoteFile) {
  //打开本地文件准备上传
  inputfile->setFileName(localFile);
  if (!inputfile->open(QIODevice::ReadOnly)) {
    throw std::runtime_error("not open file(Read Only)");
  }
  ptrUrl->setPath(remoteFile);
  curl_easy_reset(curlSession);

  //创建上传设置
  curl_easy_setopt(curlSession, CURLOPT_URL, ptrUrl->toString().toStdString().c_str());
  curl_easy_setopt(curlSession, CURLOPT_UPLOAD, 1L);
  curl_easy_setopt(curlSession, CURLOPT_FTP_CREATE_MISSING_DIRS, CURLFTP_CREATE_DIR_RETRY);
  curl_easy_setopt(curlSession, CURLOPT_INFILESIZE, inputfile->size());
  curl_easy_setopt(curlSession, CURLOPT_READFUNCTION, &ftpSession::readFileCallbask);
  curl_easy_setopt(curlSession, CURLOPT_READDATA, inputfile.get());

  CURLcode err = perform();
  inputfile->close();
  if (err != CURLE_OK) {
    DOODLE_LOG_WARN << curl_easy_strerror(err) << "-->" << inputfile->fileName();
    return false;
  }
  emit finished();
  return true;
}

oFileInfo ftpSession::fileInfo(const QString &remoteFile) {
  if (remoteFile.isEmpty()) throw std::runtime_error("remote file is NULL");
  ptrUrl->setPath(QDir::cleanPath(remoteFile));
  curl_easy_reset(curlSession);

  oFileInfo info;

  curl_easy_setopt(curlSession, CURLOPT_URL, ptrUrl->toString().toStdString().c_str());
  curl_easy_setopt(curlSession, CURLOPT_NOBODY, 1L);//不获取文件本身
  curl_easy_setopt(curlSession, CURLOPT_FILETIME, 1L);//获得文件时间
  curl_easy_setopt(curlSession, CURLOPT_HEADERFUNCTION, &ftpSession::notCallbask);
  curl_easy_setopt(curlSession, CURLOPT_HEADER, 0L);

  CURLcode err = perform();

  if (err == CURLE_OK) {
    long ftime = -1;
    err = curl_easy_getinfo(curlSession, CURLINFO_FILETIME, &ftime);
    if (err == CURLE_OK && ftime > 0)
      info.fileMtime = static_cast<time_t>(ftime);
    else
      info.fileMtime = -1;

    err = curl_easy_getinfo(curlSession, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &info.fileSize);
    if (err != CURLE_OK || info.fileSize < 0) {
      info.fileSize = 0;
      info.isFolder = true;
    } else{
      info.isFolder = false;
    }

  } else {
    info.fileMtime = -1;
    info.fileSize = 0;
    info.isFolder = true;
  }
  info.filepath = QDir::cleanPath(remoteFile);

  return info;
}
/*
获得服务器上的文件列表
*/
std::vector<oFileInfo> ftpSession::list(const QString &remoteFolder) {
  const QStringList remote = remoteFolder.split("/", QString::SkipEmptyParts);
  ptrUrl->setPath(remoteFolder);

  curl_easy_reset(curlSession);//重置保护
  QString folderList;

  curl_easy_setopt(curlSession, CURLOPT_URL, ptrUrl->toString().toStdString().c_str());
//  curl_easy_setopt(curlSession, CURLOPT_NOBODY, 1L);//不获取文件本身
//  curl_easy_setopt(curlSession, CURLOPT_FILETIME, 1L);//获得文件时间
  curl_easy_setopt(curlSession, CURLOPT_WRITEFUNCTION, &ftpSession::writeStringCallbask);
  curl_easy_setopt(curlSession, CURLOPT_WRITEDATA, &folderList);
//  curl_easy_setopt(curlSession, CURLOPT_HEADER, 0L);


  std::vector<oFileInfo> listInfo;
  CURLcode err = perform();
  if (err != CURLE_OK) {
    DOODLE_LOG_WARN << curl_easy_strerror(err);
    return listInfo;
  }
  QStringList::const_iterator iter_folder;

  QStringList list_folder = folderList.split("\r\n", QString::SkipEmptyParts);
  for (iter_folder = list_folder.begin(); iter_folder != list_folder.end(); iter_folder++) {
    oFileInfo info;
    info.isFolder = false;
    QStringList folderInfo = iter_folder->split(" ", QString::SkipEmptyParts);
    if (folderInfo[0].at(0) == "d")
      info.isFolder = true;

    //folderInfo[folderInfo.size() - 1]
    QStringList p(remote);
    p.prepend("");
    p.append(folderInfo[folderInfo.size() - 1]);
    info.filepath = p.join("/");

    if (info.isFolder)
      info.filepath += "/";
    listInfo.push_back(info);
  }
  return listInfo;
}

size_t ftpSession::writeFileCallbask(void *buff, size_t size, size_t nmemb, void *data) {
  if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1) || (data == nullptr)) return 0;
  auto *file = static_cast<QFile *>(data);
  if (file->isOpen()) {
    file->write(reinterpret_cast<char *>(buff), size * nmemb);
  }
  return size * nmemb;
}

size_t ftpSession::readFileCallbask(void *buff, size_t size, size_t nmemb, void *data) {
  auto *file = static_cast<QFile *>(data);
  return file->read(reinterpret_cast<char *>(buff), size * nmemb);
}

size_t ftpSession::notCallbask(void *buff, size_t size, size_t nmemb, void *data) {
  return size * nmemb;
}

size_t ftpSession::writeStringCallbask(void *ptr, size_t size, size_t nmemb, void *data) {
  if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1) || (data == nullptr)) return 0;
  auto *ptrlist = static_cast<QString *>(data);
  if (ptrlist != nullptr) {
    ptrlist->append(QByteArray(reinterpret_cast<char *>(ptr), size * nmemb));
    return size * nmemb;
  }
  return 0;
}

CURLcode ftpSession::perform() {
  CURLcode err;
  err = curl_easy_perform(curlSession);
  if (err != CURLE_OK)
    DOODLE_LOG_WARN << curl_easy_strerror(err);
  return err;
}
bool ftpSession::uploadFolder(const QString &localFolder, const QString &remoteFolder) {
  bool err = true;
  auto localFile_ = QDir::cleanPath(localFolder);//清理路径多余字符
  auto remoteFile_ = QDir::cleanPath(remoteFolder);

  if (QFileInfo(localFile_).isFile()) {
    err &= upload(localFile_, remoteFile_);
  } else {
    auto k_local_iter = new QDirIterator(localFile_, QDirIterator::Subdirectories);
    while (k_local_iter->hasNext()) {
      auto file = k_local_iter->filePath();
      if (QFileInfo(file).isFile()) {
        auto rem_file = file;
        err &= upload(file, rem_file.replace(localFile_, remoteFile_));
      }
      k_local_iter->next();
    }
  }
  return err;
}
bool ftpSession::downFolder(const QString &localFile, const QString &remoteFile) {
  bool err = true;
  auto localFile_ = QDir::cleanPath(localFile);//清理路径多余字符
  auto remoteFile_ = QDir::cleanPath(remoteFile);

  auto k_lo_dir = QDir(localFile_);
  if (!k_lo_dir.exists())
    k_lo_dir.mkpath(QDir::cleanPath(localFile_));

  auto k_list = list(remoteFile_ + "/");
  for (auto &&k_f : k_list) {
    //文件夹的话直接递归
    auto k_loca_path = k_f.filepath;
    if (k_f.isFolder) {
      err &= downFolder(k_loca_path.replace(remoteFile_, localFile_), k_f.filepath);
    } else {
      err &= down(k_loca_path.replace(remoteFile_, localFile_), k_f.filepath);
    }
  }
  return err;
}
FTPSPACE_E
