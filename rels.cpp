#include "rels.h"

Rels* Rels::rels_instance=nullptr;

Rels *Rels::instance()
{
    if (rels_instance==nullptr)
        rels_instance = new Rels();
    return rels_instance;
}

Rels::Rels(QObject *parent) : QObject(parent)
{
    relOwen = new DbRelation(QString("select id, num from owens order by num"),0,1,this);
    relElDim = new DbRelation(QString("select ide, fnam from dry_els order by fnam"),0,1,this);
    relRab = new DbRelation(QString("select id, snam from rab_rab order by snam"),0,1,this);
    relProg = new DbRelation(QString("select id, nam from dry_reg order by nam"),0,1,this);
}

void Rels::refresh()
{
    relOwen->refreshModel();
    relElDim->refreshModel();
    relRab->refreshModel();
    relProg->refreshModel();
}
