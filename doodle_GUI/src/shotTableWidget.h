﻿//
// Created by teXiao on 2020/10/13.
//

#pragma once

#include "doodle_global.h"

#include "core_global.h"

#include <QTableView>
#include <src/movieArchive.h>


class QProgressDialog;
DOODLE_NAMESPACE_S

class shotTableWidget : public QTableView {
 Q_OBJECT
 public:
  explicit shotTableWidget(QWidget *parent = nullptr);

  void setModel(QAbstractItemModel *model) override;
 public slots:
  void init(const doCore::shotTypePtr &file_type_ptr);
  void clear();
 private slots:
  void getSelectPath();
  void exportFbx();

  void createFlipbook_slot();
  void createFlipbook(const QString & video_folder);

 protected:
  void contextMenuEvent(QContextMenuEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragLeaveEvent(QDragLeaveEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dropEvent(QDropEvent *event) override;
 private:
  doCore::shotTypePtr p_type_ptr_;
  shotTableModel *p_model_;

  QMenu *p_menu_;

  QProgressDialog * p_dialog_;


  std::vector<doCore::mayaArchiveShotFbxPtr> exportList;
  std::vector<doCore::movieArchive> movieList;
 private:
  void insertShot(const QString &path);
  void enableBorder(const bool &isEnable);
  void openPath(const doCore::fileSqlInfoPtr &info_ptr, const bool &openEx);
};

DOODLE_NAMESPACE_E


