//
// Created by TD on 2021/7/6.
//

#pragma once
#include <Doodlelib/DoodleLib_fwd.h>
#include <Doodlelib/Gui/action/action.h>

namespace doodle {
namespace action_arg {
class DOODLELIB_API arg_path : public action::_arg {
 public:
  arg_path() = default;
  arg_path(std::vector<FSys::path>& in_paths, FSys::path& in_out_path)
      : image_list(in_paths),
        out_file(in_out_path){};
  /**
   * @brief 图片目录列表, 图片目录列表是一系列填充序列图的文件夹
   * 
   */
  std::vector<FSys::path> image_list;
  /**
   * @brief 输出路径， 没有文件名称， 文件名称由程序生成
   * 
   */
  FSys::path out_file;
};

}  // namespace action_arg

class DOODLELIB_API actn_image_to_movie : public action_indirect<action_arg::arg_path> {
 public:
  using arg_ = action_arg::arg_path;

  /**
   * @brief 将一个文件夹中的图片连接为视频
   * 
   */
  actn_image_to_movie();

  bool is_accept(const arg_& in_any) override;
  void run(const MetadataPtr& in_data, const MetadataPtr& in_parent) override;
};

}  // namespace doodle