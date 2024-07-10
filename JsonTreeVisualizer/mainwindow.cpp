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
    //Just my example files, put the path to your desired file down below
    //jsonTreeView->loadJsonFromFile("C:/Users/home/Documents/JsonTreeVisualizer/sample.json");
    jsonTreeView->loadJsonFromFile("C:/Users/home/Documents/JsonTreeVisualizer/oneMoreSample.json");
}

MainWindow::~MainWindow()
{
    delete ui;
}

