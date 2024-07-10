#include "JsonTreeView.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItem>
#include <QFile>
#include <QMessageBox>
#include <QHeaderView>

JsonTreeView::JsonTreeView(QWidget *parent) : QTreeView(parent)
{
    setModel(&m_model);
    setHeaderHidden(true);
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void JsonTreeView::setJson(const QJsonDocument &jsonDoc)
{
    m_model.clear();
    QStandardItem *rootItem = m_model.invisibleRootItem();
    if (jsonDoc.isArray()) {
        populateModel(jsonDoc.array(), rootItem);
    } else if (jsonDoc.isObject()) {
        populateModel(jsonDoc.object(), rootItem);
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Invalid JSON data format."));
    }
}

void JsonTreeView::loadJsonFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open file: %1").arg(file.errorString()));
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull()) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to parse JSON data."));
        return;
    }
    setJson(jsonDoc);
}

void JsonTreeView::populateModel(const QJsonObject &object, QStandardItem *parentItem)
{
    for (auto it = object.constBegin(); it != object.constEnd(); ++it) {
        QStandardItem *keyItem = new QStandardItem(it.key());
        QStandardItem *valueItem = new QStandardItem(it.value().toVariant().toString());
        parentItem->appendRow({ keyItem, valueItem });

        if (it.value().isObject()) {
            populateModel(it.value().toObject(), keyItem);
        } else if (it.value().isArray()) {
            populateModel(it.value().toArray(), keyItem);
        }
    }
}

void JsonTreeView::populateModel(const QJsonArray &array, QStandardItem *parentItem)
{
    for (int i = 0; i < array.size(); ++i) {
        QStandardItem *indexItem = new QStandardItem(QString("[%1]").arg(i));
        QStandardItem *valueItem = new QStandardItem(array[i].toVariant().toString());
        parentItem->appendRow({ indexItem, valueItem });

        if (array[i].isObject()) {
            populateModel(array[i].toObject(), indexItem);
        } else if (array[i].isArray()) {
            populateModel(array[i].toArray(), indexItem);
        }
    }
}

void JsonTreeView::populateModel(const QJsonValue &value, QStandardItem *parentItem)
{
    if (value.isObject()) {
        populateModel(value.toObject(), parentItem);
    } else if (value.isArray()) {
        populateModel(value.toArray(), parentItem);
    } else {
        QStandardItem *valueItem = new QStandardItem(value.toVariant().toString());
        parentItem->appendRow(valueItem);
    }
}
