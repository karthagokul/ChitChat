#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  "widget.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Demo Chat Client");
    ui->tabWidget->setTabsClosable(true);
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),SLOT(onCloseTab(int)));
    connect(ui->actionNew,SIGNAL(triggered(bool)),this,SLOT(onCreateNewChatWindow()));
    //Start with a session by default
    onCreateNewChatWindow();
    setFixedSize(size());
    QLabel *sLabel=new QLabel(this);
    sLabel->setText("Chat Client V1.0 | Gokul Kartha <kartha.gokul@gmail.com>");
    statusBar()->addWidget(sLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCloseTab(int index)
{
    QWidget *t=ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);
    t->deleteLater();
}

void MainWindow::onCreateNewChatWindow()
{
    Widget *w=new Widget(this);
    w->show();
    ui->tabWidget->addTab(w,"Connection:"+QString::number(ui->tabWidget->count()+1));
 }
