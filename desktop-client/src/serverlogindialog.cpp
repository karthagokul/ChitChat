#include "serverlogindialog.h"
#include "ui_serverlogindialog.h"
#include <QDebug>

#ifdef SEARCH_SERVER
#include "discoveryhandler.h"
#endif

ServerLoginDialog::ServerLoginDialog(QWidget *parent) :
    QDialog(parent),
    mUi(new Ui::ServerLoginDialog)
{
    setAttribute( Qt::WA_DeleteOnClose, true );
    mUi->setupUi(this);
    connect(mUi->okayButton,SIGNAL(clicked(bool)),this,SLOT(onOkayButtonClick()));
#ifdef SEARCH_SERVER
    mSearch=new DiscoveryManager(parent);
    connect(mSearch,SIGNAL(serverinfo(QString,int)),this,SLOT(onServerInfo(QString,int)));
    connect(mUi->searchButton,SIGNAL(clicked(bool)),this,SLOT(onSearchClick()));
    mSearch->init(true);
    mUi->searchButton->setVisible(true);
#else
    mUi->searchButton->setVisible(false);
#endif

}

#ifdef SEARCH_SERVER
void ServerLoginDialog::onSearchClick()
{
     mSearch->searchServer();
}

void ServerLoginDialog::onServerInfo(QString ip,int port)
{
    qDebug()<<"Host"<<ip<<"Port"<<port;
    mUi->ipLineEdit->setText(ip);
    mUi->portLineEdit->setText(QString::number(port));
}
#endif

void ServerLoginDialog::onOkayButtonClick()
{
    emit setServerData(mUi->ipLineEdit->text(),mUi->portLineEdit->text(),mUi->avatarName->text());
    close();
}

ServerLoginDialog::~ServerLoginDialog()
{
    delete mUi;
}
