//
// Created by TD on 2021/6/28.
//

#pragma once
#include <DoodleLib/DoodleLib_fwd.h>
#include <DoodleLib/Gui/action/action.h>

namespace doodle {

class DOODLELIB_API actn_assfile_create : public action_indirect<action::arg_str> {
 public:
  using arg = arg_str;

  actn_assfile_create();
  /**
   * @brief 输入的是显示名称
   * @param in_any 显示名称（std::string）
   */
  explicit actn_assfile_create(std::any&& in_any);
  void run(const MetadataPtr& in_data, const MetadataPtr& in_parent) override;
};

class DOODLELIB_API actn_assfile_add_com : public action_indirect<action::arg_str> {
 public:
  using arg = arg_str;
  actn_assfile_add_com();
  explicit actn_assfile_add_com(std::any&& in_any);
  void run(const MetadataPtr& in_data, const MetadataPtr& in_parent) override;
};

namespace action_arg {
class arg_time : public action::_arg {
 public:
  arg_time() = default;
  explicit arg_time(TimeDurationPtr in_) : time(std::move(in_)){};
  TimeDurationPtr time;
};

}  // namespace action_arg

class DOODLELIB_API actn_assfile_datetime : public action_indirect<action_arg::arg_time> {
 public:
  using arg = action_arg::arg_time;
  actn_assfile_datetime();
  explicit actn_assfile_datetime(std::any&& in_any);
  void run(const MetadataPtr& in_data, const MetadataPtr& in_parent) override;
};
class DOODLELIB_API actn_assfile_delete : public action_indirect<action::arg_null> {
 public:
  using arg = arg_null;

  actn_assfile_delete();
  explicit actn_assfile_delete(std::any&& in_any);
  void run(const MetadataPtr& in_data, const MetadataPtr& in_parent) override;
};

}  // namespace doodle
