#pragma once

#include <lib/MotionGlobal.h>

#include <lib/kernel/MotionFile.h>
#include <boost/signals2.hpp>

#include <QtCore/QAbstractListModel>
#include <QtCore/qsortfilterproxymodel.h>
namespace doodle::motion::ui {
class MotionModel : public QAbstractListModel {
  Q_OBJECT
  std::vector<kernel::MotionFilePtr> p_lists;

  void doodleBindData(const kernel::MotionFilePtr &data);

 public:
  MotionModel(QObject *parent = nullptr);

  [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
  [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
  //设置编辑标识
  [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;
  //设置数据
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;

  //插入数据
  bool insertRows(int position, int rows, const QModelIndex &index) override;
  //删除数据
  bool removeRows(int position, int rows, const QModelIndex &index) override;

  //我们自定义的插入数据
  bool insertData(int position, const kernel::MotionFilePtr &data);

  void setLists(const std::vector<kernel::MotionFilePtr> &lists);
};

class MotionModelSortFilter : public QSortFilterProxyModel {
  Q_OBJECT

 public:
  MotionModelSortFilter(QObject *parent = nullptr);

 protected:
  bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};
}  // namespace doodle::motion::ui