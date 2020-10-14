﻿#include "mainWindows.h"

#include "ProjectWidget.h"


#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>

DOODLE_NAMESPACE_S

mainWindows::mainWindows(QWidget *parent)
    : QMainWindow(parent),
      exitAction(nullptr),
      refreshAction(nullptr),
      openSetWindows(nullptr),
      p_menu_bar_(nullptr),
      p_menu_(nullptr),
      p_status_bar_(nullptr),
      centralWidget(nullptr),
      p_b_box_layout_(nullptr),
      p_prject_widght_(nullptr){
  doodle_init();
}

mainWindows::~mainWindows() = default;

void mainWindows::doodle_init() {
  //初始化自身
  if (objectName().isEmpty())
    setObjectName(QString::fromUtf8("mainWindows"));
  resize(1200, 800);
  setWindowTitle(tr("MainWindow"));

  //添加动作和菜单
  doodle_createAction();

  //设置中央小部件
  centralWidget = new QWidget(this);
  centralWidget->setObjectName(QString::fromUtf8("mainWindows"));
  //添加中央小部件
  setCentralWidget(centralWidget);

  //设置基本布局
  p_b_box_layout_ = new QHBoxLayout(centralWidget);
  p_b_box_layout_->setSpacing(0);
  p_b_box_layout_->setContentsMargins(0, 0, 0, 0);
  p_b_box_layout_->setObjectName(QString::fromUtf8("p_b_box_layout_"));

  p_prject_widght_ = new ProjectWidget(centralWidget);
  p_b_box_layout_->addWidget(p_prject_widght_);
}

void mainWindows::doodle_createAction() {
  //添加菜单栏
  p_menu_bar_ = new QMenuBar(this);
  p_menu_bar_->setObjectName(QString::fromUtf8("p_menu_bar_"));
  p_menu_bar_->setGeometry(QRect(0, 0, 640, 31));
  this->setMenuBar(p_menu_bar_);

  //添加菜单
  p_menu_ = new QMenu(p_menu_bar_);
  p_menu_->setObjectName(QString::fromUtf8("p_menu_"));
  p_menu_->setTitle(tr("&File"));
  p_menu_bar_->addAction(p_menu_->menuAction());

  //添加菜单动作
  refreshAction = new QAction(this);
  refreshAction->setObjectName(QString::fromUtf8("refreshAction"));
  refreshAction->setText(tr("Refresh"));
  refreshAction->setStatusTip(tr("刷新"));
  refreshAction->setToolTip(tr("Refresh"));
  p_menu_->addAction(refreshAction);

  openSetWindows = new QAction(this);
  openSetWindows->setObjectName(QString::fromUtf8("openSetWindows"));
  openSetWindows->setText(tr("Open Setting"));
  openSetWindows->setStatusTip(tr("打开设置"));
  openSetWindows->setToolTip(tr("Open Setting"));
  p_menu_->addAction(openSetWindows);

  exitAction = new QAction(this);
  exitAction->setObjectName(QString::fromUtf8("exitAction"));
  exitAction->setText(tr("Exit"));
  exitAction->setStatusTip(tr("退出"));
  exitAction->setToolTip(tr("Exit"));
  p_menu_->addAction(exitAction);

  //添加状态栏
  p_status_bar_ = new QStatusBar(this);
  p_status_bar_->setObjectName(QString::fromUtf8("p_status_bar_"));
  setStatusBar(p_status_bar_);
}

DOODLE_NAMESPACE_E
