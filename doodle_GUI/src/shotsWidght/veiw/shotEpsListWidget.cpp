﻿#include "shotEpsListWidget.h"

#include "src/shotsWidght/model/shotEpsListModel.h"
#include <core_doQt.h>

#include <QSpinBox>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMessageBox>

#include <iostream>

DOODLE_NAMESPACE_S

//------------------------------------------------------------------------------------
//                                         集数编辑类
//------------------------------------------------------------------------------------

episodesintDelegate::episodesintDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {
}

episodesintDelegate::~episodesintDelegate()
= default;

QWidget *episodesintDelegate::createEditor(QWidget *parent,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const {
  QSpinBox *editor = new QSpinBox(parent);
  editor->setFrame(false);
  editor->setMinimum(1);
  editor->setMaximum(9999);
  return editor;
}

void episodesintDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
  auto int_value = index.data(Qt::EditRole).toInt();

  auto *spinbox = static_cast<QSpinBox *>(editor);
  spinbox->setValue(int_value);
}

void episodesintDelegate::setModelData(QWidget *editor,
                                       QAbstractItemModel *model,
                                       const QModelIndex &index) const {
  auto *spinbox = static_cast<QSpinBox *>(editor);
  spinbox->interpretText();
  int value = spinbox->value();

  QMessageBox::StandardButton box = QMessageBox::information(static_cast<QWidget *>(this->parent()),
                                                             tr("警告:"), tr("将第 %1 集提交到服务器").arg(value),
                                                             QMessageBox::Yes | QMessageBox::Cancel);
  if (box == QMessageBox::Yes) {
    if (!model->setData(index, value, Qt::EditRole)) {
      model->removeRow(index.row(), QModelIndex());
    }
  } else
    model->removeRow(index.row(), QModelIndex());
}

void episodesintDelegate::updateEditorGeometry(QWidget *editor,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const {
  editor->setGeometry(option.rect);
}

/* ---------------------------------- 集数小部件 --------------------------------- */

shotEpsListWidget::shotEpsListWidget(QWidget *parent)
    : QListView(parent),
      p_episodesListDelegate(nullptr),
      p_eps_Menu(nullptr) {
  p_episodesListDelegate = new episodesintDelegate(this);
  setItemDelegate(p_episodesListDelegate);

  setStatusTip(tr("集数栏 注意不要添加错误的集数"));

  connect(this, &shotEpsListWidget::clicked,
          this, &shotEpsListWidget::_doodle_episodes_emit);
}

shotEpsListWidget::~shotEpsListWidget()
= default;

void shotEpsListWidget::insertEpisodes() {
  int raw = selectionModel()->currentIndex().row() + 1;
  model()->insertRow(raw, selectionModel()->currentIndex());
  //设置当前行的选择
  setCurrentIndex(p_episodesListModel->index(raw));
  edit(p_episodesListModel->index(raw));
}

void shotEpsListWidget::contextMenuEvent(QContextMenuEvent *event) {
  if (p_eps_Menu) {
    p_eps_Menu->clear();
  } else {
    p_eps_Menu = new QMenu(this);
  }

  auto add_eps = new QAction();
  connect(add_eps, &QAction::triggered,
          this, &shotEpsListWidget::insertEpisodes);
  add_eps->setText(tr("添加集数"));
  add_eps->setStatusTip(tr("添加集数"));
  p_eps_Menu->addAction(add_eps);
  if(selectionModel()->hasSelection()) {
    auto createMove = new QAction();
    createMove->setText(tr("制作整集拍屏"));
    connect(createMove, &QAction::triggered,
            this, &shotEpsListWidget::creatEpsMov);
    p_eps_Menu->addAction(createMove);
  }
  p_eps_Menu->move(event->globalPos());
  p_eps_Menu->show();
}

void shotEpsListWidget::_doodle_episodes_emit(const QModelIndex &index) {
  doCore::coreDataManager::get().setEpisodesPtr(index.data(Qt::UserRole)
                                                    .value<doCore::episodesPtr>());
  emit initEmit();
}
void shotEpsListWidget::setModel(QAbstractItemModel *model) {
  auto p_model = dynamic_cast<shotEpsListModel *>(model);
  if (p_model) p_episodesListModel = p_model;
  QAbstractItemView::setModel(model);
}
void shotEpsListWidget::creatEpsMov() {
  auto shotInfo = std::make_shared<doCore::shotFileSqlInfo>();

  const auto &kEps = selectionModel()->currentIndex().data(Qt::UserRole).value<doCore::episodesPtr>();
  if (kEps) {
    shotInfo->setEpisdes(kEps);
    auto move = std::make_unique<doCore::movieEpsArchive>(shotInfo);
    move->update();
  }
}
DOODLE_NAMESPACE_E