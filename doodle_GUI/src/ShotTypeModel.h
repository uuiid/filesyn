﻿//
// Created by teXiao on 2020/10/14.
//


#pragma once

#include "doodle_global.h"

#include "core_global.h"

#include <QAbstractListModel>


DOODLE_NAMESPACE_S

class ShotTypeModel : public QAbstractListModel {
 Q_OBJECT
 public:
  explicit ShotTypeModel(QObject *parent = nullptr);
  ~ShotTypeModel() override;
  //返回总行数
  [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

  [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
  [[nodiscard]] doCore::shotTypePtr daraRow(const QModelIndex &index) const;

  //返回标头
  [[nodiscard]] QVariant headerData(int section,
                                    Qt::Orientation Orientation,
                                    int role) const override;
  [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;
  //设置数据
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;

  //插入数据
  bool insertRows(int position, int rows, const QModelIndex &index) override;
  //删除数据
  bool removeRows(int position, int rows, const QModelIndex &index) override;

 public slots:
  void init();
  void clear();
 private:
  doCore::shotTypePtrList p_type_ptr_list_;
};
DOODLE_NAMESPACE_E