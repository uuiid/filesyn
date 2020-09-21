﻿#include "shot.h"

#include "sql_builder/sql.h"
#include "episodes.h"
#include "coreset.h"

#include <magic_enum.hpp>

#include <QVariant>

CORE_NAMESPACE_S

shot::shot()
{
    p_qint_shot_ = -1;
    p_qenm_shotab = e_shotAB::_;

    __episodes__ = -1;
}

shot::shot(const qint64 &ID_)
{
    sql::SelectModel sel_;
    sel_.select("id","shot_","shotab","__episodes__");
    sel_.from(QString("%1.shot").arg(coreSet::getCoreSet().getProjectname()).toStdString());
    sel_.where(sql::column("id") == ID_);

    sqlQuertPtr query = coreSql::getCoreSql().getquery();

    if(!query->exec(QString::fromStdString(sel_.str()))) return;
    if(query->next()){
        idP           = query->value(0).toInt();
        p_qint_shot_         = query->value(1).toInt();
        auto ab  = magic_enum::enum_cast<e_shotAB>(query->value(2).toString().toStdString());
        if(ab.has_value()){
            p_qenm_shotab = ab.value();
        }else{
            p_qenm_shotab = e_shotAB::_;
        }
        __episodes__ = query->value(2).toInt();
        return;
    }
    idP = -1;
}

void shot::setEpisdes(const episodesPtrW &value)
{
    __episodes__ = value.lock()->getIdP();
    p_ptr_eps = value;
}

episodesPtr shot::getEpisdes()
{
    if(p_ptr_eps) return p_ptr_eps.lock();
    else{
        episodesPtr p_ = episodesPtr(new episodes(__episodes__));
        p_ptr_eps = p_.toWeakRef();
        return p_;
    }
}

void shot::setShot(const qint64 &sh,const e_shotAB& ab = e_shotAB::_)
{
    p_qint_shot_ = sh;
    p_qenm_shotab = ab;
}
CORE_DNAMESPACE_E
