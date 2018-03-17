#include "widget.h"
#include "ui_widget.h"
#include  "serverlogindialog.h"
#include "clientconnection.h"
#include <QDebug>
#include <QScrollBar>
#include <QCompleter>
#include <QImageReader>

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
    connect(mUi->emotiWidget,SIGNAL(emojiSelected(QString)),this,SLOT(onEmojiSelected(QString)));

    //Lets setup the completor to the widget
    mCompletor=new QCompleter(this);
    mUi->inputTextView->setCompleter(mCompletor);
    mUi->chatView->setStyleSheet("font: 14pt;");;
    mUi->inputTextView->setStyleSheet("font: 14pt;");
    mUi->inputTextView->setFontPointSize(14);
}


void Widget::onEmojiSelected(const QString &emoji)
{
    QTextCursor cursor = mUi->inputTextView->textCursor();
    QTextImageFormat imageFormat;
    QImage image = QImageReader ( emoji ).read();
    imageFormat.setWidth( image.width() );
    imageFormat.setHeight( image.height() );
    imageFormat.setName( emoji );
    cursor.insertImage(imageFormat);

    //QString data=mUi->inputTextView->toPlainText();
    //mUi->inputTextView->setHtml(data+emoji);
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
    mCompletor->setModel(&mBuddyListModel);
}

void  Widget::onSendButtonClick()
{
    QString text=mUi->inputTextView->toHtml();
    qDebug()<<text;
    if(mUi->inputTextView->autokeywords().count()==0)
    {
        qDebug()<<"Sending to alone";
        mConnection->send(text);
    }
    else
    {
        qDebug()<<"Sending to Buddies";
        mConnection->sendToSelected(text,mUi->inputTextView->autokeywords());
    }

    mUi->inputTextView->clear();
}

void Widget::onNewMessage(QString aMessage,QString aSender)
{
    QString data=mUi->chatView->toHtml().remove("\n");
    mUi->chatView->setHtml(data+aSender+":"+aMessage);
    //Ensure to scroll down to the latest
    mUi->chatView->verticalScrollBar()->setValue(mUi->chatView->verticalScrollBar()->maximum());
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
