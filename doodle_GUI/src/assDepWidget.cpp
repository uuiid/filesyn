﻿//
// Created by teXiao on 2020/10/15.
//

#include <core_doQt.h>
#include <src/assDepModel.h>
#include <src/assDepWidget.h>

DOODLE_NAMESPACE_S
assDepWidget::assDepWidget(QWidget *parent)
    : QListView(parent), p_file_class_ass_model_(nullptr) {
  setFlow(QListView::LeftToRight);
  connect(this, &assDepWidget::clicked, this, &assDepWidget::_doodle_emit);
}
void assDepWidget::setModel(QAbstractItemModel *model) {
  auto model_ = dynamic_cast<assDepModel *>(model);
  if (model_) p_file_class_ass_model_ = model_;
  QAbstractItemView::setModel(model);
}
void assDepWidget::_doodle_emit(const QModelIndex &index) {
  auto assdep = p_file_class_ass_model_->data(index, Qt::UserRole)
                    .value<doCore::assDepPtr>();
  doCore::coreDataManager::get().setAssDepPtr(assdep);
  emit initEmit();
}
assDepWidget::~assDepWidget() = default;
DOODLE_NAMESPACE_E