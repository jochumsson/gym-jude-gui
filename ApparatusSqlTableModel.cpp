#include "ApparatusSqlTableModel.h"
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>
#include <QDebug>

ApparatusSqlTableModel::ApparatusSqlTableModel(QSqlDatabase &db):
    m_sql_model_data(db)
{
}

QAbstractTableModel * ApparatusSqlTableModel::get_qt_model()
{
    return this;
}

void ApparatusSqlTableModel::refresh()
{
    beginResetModel();
    m_sql_model_data.prepare("SELECT apparatus_name FROM apparatus");

    if (not m_sql_model_data.exec())
    {
        qWarning() << "Failed to retrive apparatus names with sql error: "
                   << m_sql_model_data.lastError().text();
        return;
    }

    endResetModel();
}

QString ApparatusSqlTableModel::get_apparatus_id(int index) const
{
    QString apparatus_id;

    if (m_sql_model_data.seek(index))
    {
        const QSqlRecord & record = m_sql_model_data.record();
        const QSqlField & field = record.field(0);
        if (not field.isNull() && field.isValid())
        {
            apparatus_id = field.value().toString();
        }
    }

    return apparatus_id;
}

int ApparatusSqlTableModel::rowCount(const QModelIndex &) const
{
    return m_sql_model_data.size();
}

int ApparatusSqlTableModel::columnCount(const QModelIndex &) const
{
    if (m_sql_model_data.first())
    {
        return m_sql_model_data.record().count();
    }

    return 0;
}

QVariant ApparatusSqlTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    const QString & apparatus_id = get_apparatus_id(index.row());
    if (not apparatus_id.isEmpty())
    {
        return m_translator.translate(apparatus_id);
    }

    return QVariant();
}