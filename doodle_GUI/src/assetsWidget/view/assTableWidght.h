﻿/*
 * @Author: your name
 * @Date: 2020-11-16 19:05:34
 * @LastEditTime: 2020-11-28 15:09:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \Doodle\doodle_GUI\src\assTableWidght.h
 */
//
// Created by teXiao on 2020/10/17.
//

#pragma once

#include <core_global.h>
#include <doodle_global.h>

#include <QTableView>
#include <future>
class QProgressDialog;
DOODLE_NAMESPACE_S
class assTableWidght : public QTableView {
  Q_OBJECT
 public:
  explicit assTableWidght(QWidget* parent = nullptr);

  void setModel(QAbstractItemModel* model) override;

 Q_SIGNALS:
  void setFilterRole(const filterState& filter);

 private:
  void init();

 private:
  assTableModel* p_model_;
  QMenu* p_menu_;

 private:
  void insertAss(const QString& path);
  void enableBorder(const bool& isEnable);

 private Q_SLOTS:
  void openFileDialog();
  void createLightDir();

  void doClickedSlots(const QModelIndex& index);
  void doDubledSlots(const QModelIndex& index);

  void deleteSQLFile();

  void chickFile(const assInfoPtr& file);

 protected:
  //上下文菜单
  void contextMenuEvent(QContextMenuEvent* event) override;

  //拖拽函数
  void dragMoveEvent(QDragMoveEvent* event) override;
  //拖拽函数
  void dragLeaveEvent(QDragLeaveEvent* event) override;
  //拖拽函数
  void dragEnterEvent(QDragEnterEvent* event) override;
  //拖拽函数
  void dropEvent(QDropEvent* event) override;
};

DOODLE_NAMESPACE_E
