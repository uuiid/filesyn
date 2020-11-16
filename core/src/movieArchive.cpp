#include "movieArchive.h"
#include "shottype.h"

#include "ffmpegWrap.h"
#include "Logger.h"

#include <QtCore/QDir>
#include <boost/filesystem.hpp>
#include <utility>
#include "shotfilesqlinfo.h"
CORE_NAMESPACE_S
movieArchive::movieArchive(fileSqlInfoPtr shot_info_ptr)
    : fileArchive(),
      p_info_ptr_(std::move(shot_info_ptr)) {

}
void movieArchive::insertDB() {
  p_info_ptr_->setFileList(p_Path);

  p_info_ptr_->insert();
}
void movieArchive::_generateFilePath() {
  if (!p_soureFile.empty())
    p_Path.push_back(p_info_ptr_->generatePath("movie", ".mp4").generic_string());
  else if (!p_info_ptr_->getFileList().empty())
    p_Path.push_back(p_info_ptr_->getFileList()[0].generic_string());
}
bool movieArchive::makeMovie(const dpath &imageFolder) {
  auto ffmpeg = std::make_unique<ffmpegWrap>(findFFmpeg() + "/ffmpeg.exe");
  auto path = dpath(imageFolder);
  dpathList list;
  for (auto &&item:boost::filesystem::directory_iterator(path)) {
    if (item.path().extension() == ".png"
        || item.path().extension() == ".jpg"
        || item.path().extension() == ".tga"
        || item.path().extension() == ".exr") {
      list.push_back(item.path().string());
    }
  }
  return ffmpeg->imageToVideo(list,
                              p_cacheFilePath.front(),
                              boost::filesystem::basename(p_cacheFilePath.front()));
}

bool movieArchive::convertMovie(const dpath &moviePath) {
  auto ffmpeg = std::make_unique<ffmpegWrap>(findFFmpeg() + "/ffmpeg.exe");
  return ffmpeg->convertToVideo(
      moviePath,
      p_cacheFilePath.front(),
      boost::filesystem::basename(p_cacheFilePath.front())
  );
}
//bool movieArchive::update(const std::vector<QString> &filelist) {
//  dpathList kdpath_list;
//  for (const auto &item : filelist) {
//    kdpath_list.push_back(item.toStdString());
//  }
//  return update(kdpath_list);
//}
bool movieArchive::update(const dpathList &filelist) {
  p_soureFile = filelist;
  DOODLE_LOG_INFO << filelist.front().c_str();
  setInfoAttr();
  _generateFilePath();
  generateCachePath();

  bool isok = false;
  if (filelist.size() == 1) {
    if (boost::filesystem::is_directory(filelist.front()))//QFileInfo().isDir()
      isok = makeMovie(filelist.front());
    else if (boost::filesystem::is_regular_file(filelist.front()))//QFileInfo().isFile()
      isok = convertMovie(filelist.front());
  } else {
    return false;
  }

  if (isok) {
    p_state_ = state::success;
    _updata(p_cacheFilePath);
    insertDB();
  } else
    p_state_ = state::fail;

  return isok;
}
dstring movieArchive::findFFmpeg() {
  auto ffmpeg_exe = QDir::current();
  ffmpeg_exe.cdUp();
  ffmpeg_exe.cd(DOODLE_FFMPEG_PATH);
  DOODLE_LOG_INFO << "找到ffmpeg" << ffmpeg_exe.absolutePath() << "\n" << ffmpeg_exe;
  return ffmpeg_exe.path().toStdString();
}

CORE_NAMESPACE_E
