#include "ActividadPorEmpleadoOptions.h"
#include "ui_ActividadPorEmpleadoOptions.h"

#include <QSqlQueryModel>

#include "DataBaseUtils.h"

ActividadPorEmpleadoOptions::ActividadPorEmpleadoOptions(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::ActividadPorEmpleadoOptions)
{
    mUi->setupUi(this);

    QSqlQueryModel* model = new QSqlQueryModel();
    QString query("SELECT * FROM ");
    query.append(DataBaseUtils::TableNames::OPERARIOS);

    model->setQuery(query);

    mUi->tableView->setModel(model);
}

ActividadPorEmpleadoOptions::~ActividadPorEmpleadoOptions()
{
    delete mUi;
}

void ActividadPorEmpleadoOptions::on_tableView_clicked(const QModelIndex &aIndex)
{
   mSelectedOperario = aIndex.sibling(aIndex.row(), 0).data().toString();
}
