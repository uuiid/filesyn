﻿//
// Created by TD on 2021/6/28.
//

#include "menu_factory.h"

#include <Gui/action/action_import.h>
#include <Metadata/AssetsFile.h>
#include <Metadata/Metadata_cpp.h>
#include <Metadata/Project.h>
#include <Metadata/Shot.h>
#include <Metadata/TimeDuration.h>
#include <core/CoreSet.h>

#include <nana/gui/filebox.hpp>
#include <nana/gui/msgbox.hpp>
#include <nana/gui/widgets/menu.hpp>
namespace doodle {

menu_factory::menu_factory(nana::window in_window)
    : p_window(in_window),
      p_set(CoreSet::getSet()),
      p_action(),
      p_metadata() {
}

void menu_factory::create_prj_action(MetadataPtr& in_metadata) {
  if (!in_metadata)
    return;
  p_metadata = in_metadata;

  auto k_create = p_action.emplace_back(std::make_shared<create_project_action>());

  k_create->sig_get_input.connect([this]() -> std::any {
    nana::folderbox mes{
        p_window};
    mes.allow_multi_select(false).title("选择项目根目录: ");

    auto k_e = mes();
    if (k_e.empty())
      return {};

    nana::inputbox mess{p_window, "项目名称: "};

    nana::inputbox::text name{"项目名称: "};
    mess.show_modal(name);
    auto str = name.value();
    if (str.empty())
      return {};

    return std::make_any<std::tuple<std::string, FSys::path> >(
        std::make_tuple(str, k_e.at(0)));
  });

  //----------------------------------------------------------------

  p_action.emplace_back(action_ptr{});

  auto k_rm = p_action.emplace_back(std::make_shared<rename_project_action>());
  auto k_rp = p_action.emplace_back(std::make_shared<setpath_project_action>());

  k_rm->sig_get_input.connect([this]() -> std::any {
    nana::inputbox mess{p_window, "项目名称: "};
    nana::inputbox::text name{"项目名称: "};
    mess.show_modal(name);
    auto str = name.value();
    if (str.empty())
      return {};
    return std::make_any<std::string>(str);
  });

  k_rp->sig_get_input.connect([this]() -> std::any {
    nana::folderbox mes{
        p_window};
    mes.allow_multi_select(false).title("选择项目根目录: ");

    auto k_e = mes();
    if (k_e.empty())
      return {};
    return std::make_any<FSys::path>(k_e.at(0));
  });
}
void menu_factory::create_file_action(MetadataPtr& in_metadata) {
  if (!in_metadata)
    return;
  p_metadata = in_metadata;

  auto k_c_f = p_action.emplace_back(std::make_shared<assfile_create_action>());
  k_c_f->sig_get_input.connect([this]() -> std::any {
    nana::inputbox msg{p_window, "创建: "};
    nana::inputbox::text name{"名称: "};
    msg.show_modal(name);
    return std::make_any<std::string>(name.value());
  });

  if (!details::is_class<AssetsFile>(in_metadata)) {
    DOODLE_LOG_WARN("不是 AssFile类， 无法设置属性");
    return;
  }

  p_action.emplace_back(action_ptr{});

  auto k_s_c = p_action.emplace_back(std::make_shared<assfile_add_com_action>());
  auto k_s_t = p_action.emplace_back(std::make_shared<assfile_datetime_action>());

  k_s_c->sig_get_input.connect([this]() -> std::any {
    nana::inputbox msg{p_window, "评论: "};
    nana::inputbox::text name{"评论: "};
    msg.show_modal(name);
    return std::make_any<std::string>(name.value());
  });
  k_s_t->sig_get_input.connect([this,in_metadata]() -> std::any {
    nana::inputbox msg{p_window, "修改: "};
    //    nana::inputbox::date k_date{"时间: "};
    auto time = std::dynamic_pointer_cast<AssetsFile>(in_metadata)->getTime();
    nana::inputbox::integer k_year{"年", time->get_year(), 1999, 2999, 1};
    nana::inputbox::integer k_month{"月",time->get_month(), 1, 12, 1};
    nana::inputbox::integer k_day{"天", time->get_day(), 1, 31, 1};
    nana::inputbox::integer k_hours{"时", time->get_hour(), 0, 23, 1};
    nana::inputbox::integer k_minutes{"分", time->get_minutes(), 0, 59, 1};
    nana::inputbox::integer k_seconds{"秒", time->get_second(), 0, 59, 1};

    msg.show_modal(k_year, k_month, k_day, k_hours, k_minutes, k_seconds);

    time->set_year(k_year.value());
    time->set_month(k_month.value());
    time->set_day(k_day.value());
    time->set_hour(k_hours.value());
    time->set_minutes(k_minutes.value());
    time->set_second(k_seconds.value());
    return std::make_any<TimeDurationPtr>(time);
  });
  auto k_s_d = p_action.emplace_back(std::make_shared<assfile_delete_action>());
}
void menu_factory::create_ass_action(MetadataPtr& in_metadata) {
  if (!in_metadata)
    return;
  p_metadata = in_metadata;

  auto k_c_ass  = p_action.emplace_back(std::make_shared<assset_create_action>());
  auto k_c_eps  = p_action.emplace_back(std::make_shared<episode_create_action>());
  auto k_c_shot = p_action.emplace_back(std::make_shared<shot_create_action>());

  k_c_ass->sig_get_input.connect([this]() -> std::any {
    nana::inputbox msg{p_window, "创建: "};
    nana::inputbox::text name{"名称: "};
    msg.show_modal(name);
    return std::make_any<std::string>(name.value());
  });
  k_c_eps->sig_get_input.connect([this]() -> std::any {
    nana::inputbox msg{p_window, "创建: "};
    nana::inputbox::integer k_i{"集数: ", 0, 0, 9999, 1};
    msg.show_modal(k_i);
    return std::make_any<std::int32_t>(k_i.value());
  });
  k_c_shot->sig_get_input.connect([this]() -> std::any {
    nana::inputbox msg{p_window, "创建: "};
    nana::inputbox::integer k_i{"镜头号: ", 0, 0, 9999, 1};
    msg.show_modal(k_i);
    return std::make_any<std::int32_t>(k_i.value());
  });

  p_action.emplace_back(action_ptr{});
  if (details::is_class<Assets>(in_metadata)) {
    auto k_s_ass = p_action.emplace_back(std::make_shared<assets_setname_action>());
    k_s_ass->sig_get_input.connect([this]() -> std::any {
      nana::inputbox msg{p_window, "创建: "};
      nana::inputbox::text name{"名称: "};
      msg.show_modal(name);
      return std::make_any<std::string>(name.value());
    });

  } else if (details::is_class<Episodes>(in_metadata)) {
    auto k_s_eps = p_action.emplace_back(std::make_shared<episode_set_action>());
    k_s_eps->sig_get_input.connect([this]() -> std::any {
      nana::inputbox msg{p_window, "修改: "};
      nana::inputbox::integer k_i{"集数: ", 0, 0, 9999, 1};
      msg.show_modal(k_i);
      return std::make_any<std::int32_t>(k_i.value());
    });

  } else if (details::is_class<Shot>(in_metadata)) {
    auto k_s_sh   = p_action.emplace_back(std::make_shared<shot_set_action>());
    auto k_s_shab = p_action.emplace_back(std::make_shared<shotab_set_action>());

    k_s_sh->sig_get_input.connect([this]() -> std::any {
      nana::inputbox msg{p_window, "修改: "};
      nana::inputbox::integer k_i{"镜头号: ", 0, 0, 9999, 1};
      msg.show_modal(k_i);
      return std::make_any<std::int32_t>(k_i.value());
    });
    k_s_shab->sig_get_input.connect([this]() -> std::any {
      nana::inputbox msg{p_window, "修改: "};
      auto k_s_v = magic_enum::enum_names<Shot::ShotAbEnum>();
      nana::inputbox::text k_text{"ab镜头号:", std::vector<std::string>{k_s_v.begin(), k_s_v.end()}};
      msg.show_modal(k_text);
      return std::make_any<std::string>(k_text.value());
    });
  }

  p_action.emplace_back(action_ptr{});
  auto k_d_ = p_action.emplace_back(std::make_shared<assets_delete_action>());
}

void menu_factory::operator()(nana::menu& in_menu) {
  MetadataPtr k_ptr         = p_metadata;
  decltype(p_window) k_item = p_window;

  for (auto& k_i : p_action) {
    if (k_i)
      in_menu.append(
          k_i->class_name(),
          [k_i, k_ptr, k_item](const nana::menu::item_proxy&) {
            try {
              (*k_i)(k_ptr);
            } catch (DoodleError& error) {
              DOODLE_LOG_WARN(error.what())
              nana::msgbox k_msgbox{k_item, error.what()};
              k_msgbox.show();
            }
          });
    else
      in_menu.append_splitter();
  }
}
}  // namespace doodle