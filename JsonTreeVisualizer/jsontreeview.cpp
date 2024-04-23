#include "JsonTreeView.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItem>
#include <QFile>
#include <QMessageBox>

JsonTreeView::JsonTreeView(QWidget *parent) : QTreeView(parent)
{
    setModel(&m_model);
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
        if (it.value().isObject() || it.value().isArray()) {
            parentItem->appendRow(keyItem);
            populateModel(it.value(), keyItem);
        } else {
            QList<QStandardItem *> items;
            items << keyItem << new QStandardItem(it.value().toVariant().toString());
            parentItem->appendRow(items);
        }
    }
}

//ToDo: Fix incorrect value parsing when facing arrays
void JsonTreeView::populateModel(const QJsonArray &array, QStandardItem *parentItem)
{
    for (int i = 0; i < array.size(); ++i) {
        QStandardItem *item = new QStandardItem(QString("[%1]").arg(i));
        parentItem->appendRow(item);
        if (array[i].isObject() || array[i].isArray()) {
            populateModel(array[i].toObject(), item);
        } else {
            QList<QStandardItem *> items;
            items << item;
            items << new QStandardItem(array[i].toVariant().toString());
            parentItem->appendRow(items);
        }
    }
}

void JsonTreeView::populateModel(const QJsonValue &value, QStandardItem *parentItem)
{
    if (value.isArray()) {
        const QJsonArray array = value.toArray();
        for (int i = 0; i < array.size(); ++i) {
            QStandardItem *item = new QStandardItem(QString("[%1]").arg(i));
            parentItem->appendRow(item);
            populateModel(array[i], item);
        }
    } else if (value.isObject()) {
        const QJsonObject obj = value.toObject();
        for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
            QStandardItem *keyItem = new QStandardItem(it.key());
            if (it.value().isObject() || it.value().isArray()) {
                parentItem->appendRow(keyItem);
                populateModel(it.value(), keyItem);
            } else {
                QList<QStandardItem *> items;
                items << keyItem << new QStandardItem(it.value().toVariant().toString());
                parentItem->appendRow(items);
            }
        }
    } else {
        QStandardItem *item = new QStandardItem(value.toVariant().toString());
        parentItem->appendRow(item);
    }
}
