#include <DoodleLib/Exception/Exception.h>
#include <DoodleLib/Logger/Logger.h>
#include <DoodleLib/PinYin/convert.h>
#include <DoodleLib/core/CoreSet.h>
#include <DoodleLib/core/CoreSql.h>
#include <DoodleLib/rpc/RpcClient.h>
#include <ShlObj.h>
#include <grpcpp/grpcpp.h>
#include <sqlpp11/mysql/mysql.h>

#include <boost/algorithm/string.hpp>
#include <boost/dll.hpp>
#include <boost/format.hpp>
#include <boost/process.hpp>
#include <boost/regex.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <magic_enum.hpp>
#include <nlohmann/json.hpp>

DOODLE_NAMESPACE_S

CoreSet &CoreSet::getSet() {
  static CoreSet install;
  return install;
}

void CoreSet::guiInit() {
  findMaya();
  getSetting();

  ///触发一次 CoreSql 初始化并测试数据库连接
  auto &k_sql   = CoreSql::Get();
  auto test_sql = k_sql.getConnection();

  boost::format ip_ch{"%s:%i"};
  ip_ch % "localhost" % p_meta_rpc_port;
  DOODLE_LOG_DEBUG(ip_ch.str())

  p_rpc_clien = std::make_shared<RpcClient>(
      grpc::CreateChannel(ip_ch.str(),
                          grpc::InsecureChannelCredentials()));

  p_matadata_setting_.init();
}

void CoreSet::reInit() {
}

void CoreSet::findMaya() {
  if (FSys::exists(R"(C:\Program Files\Autodesk\Maya2020\bin)")) {
    p_mayaPath = R"(C:\Program Files\Autodesk\Maya2020\bin\)";
  } else if (FSys::exists(R"(C:\Program Files\Autodesk\Maya2019\bin)")) {
    p_mayaPath = R"(C:\Program Files\Autodesk\Maya2019\bin\)";
  } else if (FSys::exists(R"(C:\Program Files\Autodesk\Maya2018\bin)")) {
    p_mayaPath = R"(C:\Program Files\Autodesk\Maya2018\bin\)";
  }
}

bool CoreSet::hasMaya() const noexcept {
  return !p_mayaPath.empty();
}

const FSys::path &CoreSet::MayaPath() const noexcept {
  return p_mayaPath;
}

void CoreSet::setMayaPath(const FSys::path &in_MayaPath) noexcept {
  p_mayaPath = in_MayaPath;
}

void CoreSet::writeDoodleLocalSet() {
  p_ue4_setting.testValue();
  if (p_ue4_setting.hasPath() && !FSys::exists(p_ue4_setting.Path() / DOODLE_UE_PATH)) {
    p_ue4_setting.setPath({});
    throw FileError{p_ue4_setting.Path(), " 在路径中没有找到ue,不保存"};
  }
  if (!FSys::exists(p_mayaPath / "maya.exe")) {
    throw FileError{p_mayaPath, " 在路径中没有找到maya,不保存"};
  }

  FSys::ofstream outjosn{p_doc / configFileName(), std::ios::out | std::ios::binary};
  cereal::PortableBinaryOutputArchive out{outjosn};
  out(*this);
}

void CoreSet::getSetting() {
  static FSys::path k_settingFileName = p_doc / configFileName();
  if (FSys::exists(k_settingFileName)) {
    FSys::path strFile(k_settingFileName);
    FSys::ifstream inJosn{k_settingFileName, std::ifstream::binary};

    cereal::PortableBinaryInputArchive incereal{inJosn};
    incereal(*this);
  }
}
CoreSet::CoreSet()
    : p_user_("user"),
      p_department_(Department::VFX),
      p_cache_root("C:/Doodle_cache"),
      p_doc("C:/Doodle_cache"),
      p_uuid_gen(),
      p_ue4_setting(Ue4Setting::Get()),
      p_matadata_setting_(MetadataSet::Get()),
      p_mayaPath(),
#ifdef NDEBUG
      p_server_host("192.168.10.215"),
#else
      p_server_host("192.168.10.213"),
#endif
      p_sql_port(3306),
      p_meta_rpc_port(60999),
      p_file_rpc_port(60998),
      p_sql_host("192.168.10.215"),
      p_sql_user("deve"),
      p_sql_password("deve") {
  ///这里我们手动做一些工作
  ///获取环境变量 FOLDERID_Documents
  PWSTR pManager;
  SHGetKnownFolderPath(FOLDERID_Documents, NULL, nullptr, &pManager);
  if (!pManager)
    throw DoodleError("无法找到保存路径");

  p_doc = FSys::path{pManager} / "doodle";
  CoTaskMemFree(pManager);

  if (!FSys::exists(p_doc))
    FSys::create_directories(p_doc);
  getCacheDiskPath();
  if (!FSys::exists(getCacheRoot())) {
    FSys::create_directories(getCacheRoot());
  }
}

FSys::path CoreSet::toIpPath(const FSys::path &path) {
  std::wstring str{};
  str.resize(MAX_PATH);
  DWORD dwResult, cchBuff = str.size();
  dwResult = WNetGetConnection(path.generic_wstring().c_str(), str.data(), &cchBuff);
  switch (dwResult) {
    case NO_ERROR:
      break;
    case ERROR_BUFFER_OVERFLOW: {
      str.resize(cchBuff);
      dwResult = WNetGetConnection(path.generic_wstring().c_str(), str.data(), &cchBuff);
      break;
    }
    default: {
      throw DoodleError{"错误代码：" + std::to_string(dwResult)};
    } break;
  }
  return {str};
}

boost::uuids::uuid CoreSet::getUUID() {
  return p_uuid_gen();
}

std::string CoreSet::getDepartment() const {
  return std::string{magic_enum::enum_name(p_department_)};
}

const Department &CoreSet::getDepartmentEnum() const {
  return p_department_;
}

void CoreSet::setDepartment(const std::string &value) {
  p_department_ = magic_enum::enum_cast<Department>(value).value_or(Department::VFX);
}

std::string CoreSet::getUser() const { return p_user_; }

std::string CoreSet::getUser_en() const {
  return boost::algorithm::to_lower_copy(
      convert::Get().toEn(p_user_));
}

void CoreSet::setUser(const std::string &value) {
  p_user_ = value;
}

FSys::path CoreSet::getDoc() const { return p_doc; }

FSys::path CoreSet::getCacheRoot() const {
  return p_cache_root;
}

FSys::path CoreSet::getCacheRoot(const FSys::path &in_path) const {
  auto path = p_cache_root / in_path;
  if (!FSys::exists(path))
    FSys::create_directories(path);
  return path;
}

void CoreSet::setCacheRoot(const FSys::path &path) {
  p_cache_root = path;
}

void CoreSet::getCacheDiskPath() {
  const static std::vector<std::string> dirs{"D:/",
                                             "E:/",
                                             "F:/",
                                             "G:/",
                                             "H:/",
                                             "I:/",
                                             "J:/",
                                             "K:/",
                                             "L:/"};
  for (auto &dir : dirs) {
    if (FSys::exists(dir)) {
      auto info = FSys::space(dir);
      if (((float)info.available / (float)info.available) > 0.05) {
        p_cache_root = dir + "Doodle_cache";
        break;
      }
    }
  }
}

FSys::path CoreSet::program_location() {
  return {boost::dll::program_location().parent_path().generic_string()};
}
FSys::path CoreSet::program_location(const FSys::path &path) {
  return program_location() / path;
}
std::string CoreSet::configFileName() {
  static std::string str{"doodle_config.bin"};
  return str;
}
std::string CoreSet::getUUIDStr() {
  return boost::uuids::to_string(getUUID());
}
RpcClientPtr CoreSet::getRpcChild() const {
  return p_rpc_clien;
}
void CoreSet::clear() {
  p_rpc_clien.reset();
  p_matadata_setting_.clear();
}
int CoreSet::getSqlPort() const {
  return p_sql_port;
}
void CoreSet::setSqlPort(int in_sqlPort) {
  p_sql_port = in_sqlPort;
}
const std::string &CoreSet::getSqlHost() const {
  return p_sql_host;
}
void CoreSet::setSqlHost(const std::string &in_sqlHost) {
  p_sql_host = in_sqlHost;
}
const std::string &CoreSet::getSqlUser() const {
  return p_sql_user;
}
void CoreSet::setSqlUser(const std::string &in_sqlUser) {
  p_sql_user = in_sqlUser;
}
const std::string &CoreSet::getSqlPassword() const {
  return p_sql_password;
}
void CoreSet::setSqlPassword(const std::string &in_sqlPassword) {
  p_sql_password = in_sqlPassword;
}
int CoreSet::getMetaRpcPort() const {
  return p_meta_rpc_port;
}
void CoreSet::setMetaRpcPort(int in_metaRpcPort) {
  p_meta_rpc_port = in_metaRpcPort;
}
int CoreSet::getFileRpcPort() const {
  return p_file_rpc_port;
}
void CoreSet::setFileRpcPort(int in_fileRpcPort) {
  p_file_rpc_port = in_fileRpcPort;
}

DOODLE_NAMESPACE_E