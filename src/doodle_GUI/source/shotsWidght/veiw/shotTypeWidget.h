﻿//
// Created by teXiao on 2020/10/12.
//

#pragma once

#include "doodle_global.h"

#include "core_global.h"

#include <QAbstractListModel>
#include <QStyledItemDelegate>
#include <QListView>

#include <boost/signals2.hpp>

DOODLE_NAMESPACE_S
//-----------------------------------自定义委托---------------------------------------------//
class fileTypeShotDelegate : public QStyledItemDelegate {
  Q_OBJECT

 public:
  explicit fileTypeShotDelegate(QObject *parent = nullptr);
  ~fileTypeShotDelegate() override;

  QWidget *createEditor(QWidget *parent,
                        const QStyleOptionViewItem &option_view_item,
                        const QModelIndex &index) const override;
  void setEditorData(QWidget *editor,
                     const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const override;
};

//-----------------------------------自定义小部件---------------------------------------------//
class shotTypeWidget : public QListView {
  Q_OBJECT
 public:
  explicit shotTypeWidget(QWidget *parent = nullptr);
  ~shotTypeWidget() override;

  void clear();
  boost::signals2::signal<void(const shotTypePtr &, const filterState &)> doodleUseFilter;

 private:
  //模型
  shotTypeModel *p_file_type_shot_model_;
  //委托
  fileTypeShotDelegate *p_file_type_shot_delegate_;
  //上下文菜单
  QMenu *p_menu_;
  //上一级发射出来的指针
  shotClassPtr p_file_class_ptr_;
 private Q_SLOTS:
  //添加filetype
  void insertFileType();
  //发射fileType
  void _doodle_chicked_emit(const QModelIndex &index);

 protected:
  void mousePressEvent(QMouseEvent *event) override;
  //void contextMenuEvent(QContextMenuEvent *event) override;
};

DOODLE_NAMESPACE_E