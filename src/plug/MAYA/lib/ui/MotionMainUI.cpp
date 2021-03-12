#include <lib/ui/MotionMainUI.h>

#include <lib/kernel/MotionSetting.h>

#include <maya/MGlobal.h>

#include <lib/ui/MotionSettingWidget.h>
#include <lib/ui/MotionLibWidget.h>

#include <QtWidgets/QGridLayout.h>
#include <QtWidgets/QPushButton.h>

#include <QtWidgets/qstatusbar.h>
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qmenubar.h>
namespace doodle::motion::ui {

MotionMainUI::MotionMainUI(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
      p_layout(),
      p_centralWidget(),
      p_setting_widget() {
  resize(1200, 800);

  setWindowTitle("doodle动作库");

  //设置中央小部件
  p_centralWidget = new QWidget(this);
  p_centralWidget->setObjectName("doodleMotionCentralWidght");
  setCentralWidget(p_centralWidget);
  //创建布局
  p_layout = new QGridLayout(p_centralWidget);

  //创建设置面板
  p_setting_widget = new doodle::motion::ui::MotionSettingWidget(p_centralWidget);
  auto rootPath    = doodle::motion::kernel::MotionSetting::Get().MotionLibRoot();
  //这里我们使设置成为一个窗口
  p_setting_widget->setWindowFlags(Qt::Window);
  if (rootPath.empty()) {
    openSettingMotionLib();
  }
  openMotionLib();

  createMenu();
}
MotionMainUI::~MotionMainUI() {
  // delete p_setting_widget;
}

void MotionMainUI::openSettingMotionLib() {
  //断开上次的所有链接
  // disconnect(p_setting_widget,
  //            &doodle::motion::ui::MotionSettingWidget::ReturnUp,
  //            nullptr, nullptr);
  //链接返回代码
  connect(p_setting_widget,
          &doodle::motion::ui::MotionSettingWidget::ReturnUp,
          this, [=]() {
            doodle::motion::kernel::MotionSetting::Get().save();
            p_setting_widget->hide();
          });
  p_setting_widget->resize(800, 1200);
  p_setting_widget->show();
}

void MotionMainUI::openMotionLib() {
  auto k_motion = new MotionLibWidget();
  p_layout->addWidget(k_motion);
  k_motion->show();
}

void MotionMainUI::createMenu() {
  auto k_status_bar = new QStatusBar();
  k_status_bar->setObjectName("k_status_bar");

  setStatusBar(k_status_bar);

  auto k_menu_bar = new QMenuBar();
  k_status_bar->setObjectName("k_menu_bar");
  this->setMenuBar(k_menu_bar);

  auto k_menu_file = k_menu_bar->addMenu(tr("文件"));
  k_menu_file->setObjectName("k_menu_file");

  auto k_save_action        = k_menu_file->addAction(tr("保存"));
  auto k_open_stting_action = k_menu_file->addAction(tr("打开设置"));
  k_menu_file->addSeparator();
  auto k_close_action = k_menu_file->addAction(tr("关闭"));

  //链接保存按钮
  connect(k_save_action, &QAction::triggered,
          this, [=]() { doodle::motion::kernel::MotionSetting::Get().save(); });
  //链接打开设置按钮
  connect(k_open_stting_action, &QAction::triggered,
          p_setting_widget, &MotionSettingWidget::show);
  //连接关闭按钮
  connect(k_close_action, &QAction::triggered,
          this, &MotionMainUI::close);
}

}  // namespace doodle::motion::ui