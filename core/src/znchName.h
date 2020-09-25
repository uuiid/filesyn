#pragma once

#include "core_global.h"

#include "coresqldata.h"

#include "pinyin_global.h"

CORE_NAMESPACE_S

class CORE_EXPORT znchName :public coresqldata
{
public:
    znchName(const assTypePtr & at_);
    ~znchName();

    void insert() override;
    void updateSQL() override;
    void deleteSQL() override;
    void select(const assTypePtr & at_);

    void setName(const QString & name_);
    void setName(const QString & name_, const bool & isZNCH);
    QString getName() const;
    QString pinyin() const;
private:
    QString nameZNCH;
    QString nameEN;
    dopinyin::convertPtr con;

    assTypePtrW p_ptr_assType;
};



CORE_DNAMESPACE_E