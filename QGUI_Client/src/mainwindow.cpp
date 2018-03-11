#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  "widget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Demo Chat Client");
    connect(ui->actionNew,SIGNAL(triggered(bool)),this,SLOT(onCreateNewChatWindow()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCreateNewChatWindow()
{
    Widget *w=new Widget(this);
    w->show();
    ui->tabWidget->addTab(w,"Connection:"+QString::number(ui->tabWidget->count()+1));
 }
