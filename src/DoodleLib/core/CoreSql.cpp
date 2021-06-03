
#include <DoodleLib/Exception/Exception.h>
#include <DoodleLib/Metadata/Project.h>
#include <DoodleLib/core/CoreSql.h>
#include <DoodleLib/core/CoreSet.h>

#include <sqlpp11/mysql/mysql.h>
#include <sqlpp11/sqlpp11.h>

DOODLE_NAMESPACE_S
CoreSql::CoreSql()
    : config(std::make_shared<sqlpp::mysql::connection_config>()) {
  Init();
}

void CoreSql::Init() {
  auto& set = CoreSet::getSet();

  config->port     = set.getSqlPort();
  config->host     = set.getSqlHost();
  config->user     = set.getSqlUser();
  config->password = set.getSqlPassword();
#ifdef NDEBUG
  config->debug    = false;
  config->database = "doodle";
#else
  config->debug    = true;
  config->database = "doodle_test";
#endif
}

ConnPtr CoreSql::getConnection() const {
  return std::make_unique<sqlpp::mysql::connection>(config);
}
CoreSql& CoreSql::Get() {
  static CoreSql install;
  return install;
}

DOODLE_NAMESPACE_E