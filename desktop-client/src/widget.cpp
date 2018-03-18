#include "widget.h"
#include "ui_widget.h"
#include  "serverlogindialog.h"
#include "clientconnection.h"
#include <QDebug>
#include <QScrollBar>
#include <QCompleter>
#include <QImageReader>
#include<QTextBlockFormat>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::Widget),mConnection(new ClientConnection(this))
{
    setAttribute( Qt::WA_DeleteOnClose, true );
    mUi->setupUi(this);
    connect(mUi->settingsButton,SIGNAL(clicked(bool)),this,SLOT(showLoginDialog()));
    connect(mUi->logonButton,SIGNAL(clicked(bool)),this,SLOT(onLogonButtonClick()));
    connect(mUi->sendButton,SIGNAL(clicked(bool)),this,SLOT(onSendButtonClick()));
    connect(mConnection,SIGNAL(stateChanged()),this,SLOT(onSessionStateChanged()));
    connect(mConnection,SIGNAL(buddylist(QString)),this,SLOT(onBuddyList(QString)));
    connect(mConnection,SIGNAL(newMessage(QString,QString,bool)),this,SLOT(onNewMessage(QString,QString,bool)));
    connect(mConnection,SIGNAL(error(QString)),this,SLOT(onError(QString)));
    connect(mUi->emotiWidget,SIGNAL(emojiSelected(QString)),this,SLOT(onEmojiSelected(QString)));

    //Lets setup the completor to the widget
    mCompletor=new QCompleter(this);
    mUi->inputTextView->setCompleter(mCompletor);

    //Disable send button until connected.
    mUi->sendButton->setEnabled(true);

    mUi->chatView->setReadOnly(true);
    mUi->chatView->setTextInteractionFlags(Qt::NoTextInteraction);
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
        mConnection->disconnectFromServer();
        //Clear the List of Users
        mBuddyListModel.setStringList(QStringList());
        mUi->buddyListView->setModel(&mBuddyListModel);
    }
    else
    {
        mConnection->connectNow();
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

        onNewMessage("There is no Active Connection","System",false);
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
    if(!aMessage.isEmpty())
        onNewMessage(aMessage,"System",false);

    mCompletor->setModel(&mBuddyListModel);
}

void  Widget::onSendButtonClick()
{
    QString text=mUi->inputTextView->toHtml();
    qDebug()<<text;
    if(mUi->inputTextView->autokeywords().count()==0)
    {
        //qDebug()<<"Sending to alone";
        mConnection->send(text);
    }
    else
    {
        //qDebug()<<"Sending to Buddies";
        mConnection->sendToSelected(text,mUi->inputTextView->autokeywords());
    }

    mUi->inputTextView->clear();
}

void Widget::onNewMessage(QString aMessage,QString aSender,bool emphasise)
{
    QString senderText;
    if(emphasise)
    {
        aSender=aSender.append("[Privately]->:");
        senderText=QString("<p2 style=' font-weight: bold;color:red;'>") + \
                QDateTime::currentDateTime().toString("hh:mm:ss") + \
                QString(" ") + \
                aSender+QString("</p2>");
    }
    else
    {
        aSender=aSender.append(" : ");
        senderText=QString("<p2 style=' font-weight: bold;color:blue;'>")+ \
                QDateTime::currentDateTime().toString("hh:mm:ss") + \
                QString(" ") + \
                aSender+QString("</p2>");
    }

    QTextCursor cursor = mUi->chatView->textCursor();
    QTextBlockFormat format;
    format.setBackground(Qt::transparent);
    cursor.setBlockFormat(format);
    cursor.insertHtml(senderText);
    cursor.insertHtml(aMessage);
    cursor.insertHtml("<br>");

    //Ensure to scroll down to the latest
    mUi->chatView->verticalScrollBar()->setValue(mUi->chatView->verticalScrollBar()->maximum());
}

void Widget::onError(QString aMessage)
{
    //mUi->chatView->append("Error:"+aMessage);
}

Widget::~Widget()
{
    mConnection->disconnectFromServer();
    mConnection->terminate();
}
