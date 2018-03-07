#include "widget.h"
#include "ui_widget.h"
#include  "serverlogindialog.h"
#include "clientconnection.h"
#include <QDebug>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::Widget),mConnection(new ClientConnection(this))
{
    mUi->setupUi(this);
    connect(mUi->settingsButton,SIGNAL(clicked(bool)),this,SLOT(showLoginDialog()));
    connect(mUi->logonButton,SIGNAL(clicked(bool)),this,SLOT(onLogonButtonClick()));
    connect(mUi->sendButton,SIGNAL(clicked(bool)),this,SLOT(onSendButtonClick()));
    connect(mConnection,SIGNAL(stateChanged()),this,SLOT(onSessionStateChanged()));
    connect(mConnection,SIGNAL(buddylist(QString)),this,SLOT(onBuddyList(QString)));
    connect(mConnection,SIGNAL(newMessage(QString,QString)),this,SLOT(onNewMessage(QString,QString)));
    connect(mConnection,SIGNAL(error(QString)),this,SLOT(onError(QString)));
}

void Widget::showLoginDialog()
{
    ServerLoginDialog *dialog=new ServerLoginDialog(this);
    connect(dialog,SIGNAL(setServerData(QString,QString,QString)),this,SLOT(onSetServerData(QString,QString,QString)));
    dialog->show();
}

void Widget::onLogonButtonClick()
{
    if(mConnection->isActive())
    {
        mConnection->stop();
        //Clear the List of Users
        mBuddyListModel.setStringList(QStringList());
        mUi->buddyListView->setModel(&mBuddyListModel);
    }
    else
    {
        mConnection->start();
    }
}

void Widget::onSessionStateChanged()
{
    if(mConnection->isActive())
    {
        mUi->logonButton->setText("Log Off");
        mUi->sendButton->setEnabled(true);
    }
    else
    {
        mUi->logonButton->setText("Log On");
        mUi->sendButton->setEnabled(false);
    }
}

void Widget::onSetServerData(QString aHostIp ,QString aPort,QString aAvatarName)
{
    mConnection->setServer(aHostIp,aPort.toInt());
    mConnection->setUserName(aAvatarName);
}

void Widget::onBuddyList(QString aMessage)
{
    mBuddyListModel.setStringList(mConnection->buddies());
    mUi->buddyListView->setModel(&mBuddyListModel);
    mUi->chatView->append(aMessage);
}

void  Widget::onSendButtonClick()
{
    QString text=mUi->inputTextView->toPlainText();
    mConnection->send(text);
    mUi->inputTextView->clear();
}

void Widget::onNewMessage(QString aMessage,QString aSender)
{
    //mUi->chatView->append("\n");
    mUi->chatView->append(aSender+":"+aMessage);

}

void Widget::onError(QString aMessage)
{
    mUi->chatView->append("Error:"+aMessage);
}

Widget::~Widget()
{
    delete mConnection;
    delete mUi;
}
