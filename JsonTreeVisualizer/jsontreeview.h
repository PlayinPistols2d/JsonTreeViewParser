#ifndef JSONTREEVIEW_H
#define JSONTREEVIEW_H

#include <QTreeView>
#include <QJsonDocument>
#include <QStandardItemModel>

class JsonTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit JsonTreeView(QWidget *parent = nullptr);
    void setJson(const QJsonDocument &jsonDoc);
    void loadJsonFromFile(const QString &filePath);

private:
    void populateModel(const QJsonObject &object, QStandardItem *parentItem);
    void populateModel(const QJsonArray &array, QStandardItem *parentItem);
    void populateModel(const QJsonValue &value, QStandardItem *parentItem);

    QStandardItemModel m_model;
};

#endif // JSONTREEVIEW_H
