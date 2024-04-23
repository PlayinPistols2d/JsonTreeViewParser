#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("JsonTreeView - test example");

    jsonTreeView = new JsonTreeView();
    ui->vl_mainLayout->addWidget(jsonTreeView);
    //jsonTreeView->loadJsonFromFile("C:/Users/home/Documents/JsonTreeVisualizer/sample.json");
    jsonTreeView->loadJsonFromFile("C:/Users/home/Documents/JsonTreeVisualizer/oneMoreSample.json"); // to be fixed
}

MainWindow::~MainWindow()
{
    delete ui;
}

