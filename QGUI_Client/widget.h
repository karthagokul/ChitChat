#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QStringListModel>


namespace Ui {
class Widget;
}

class ClientConnection;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void showLoginDialog();
    void onSetServerData(QString aHostIp ,QString aPort,QString aAvatarName);
    void onLogonButtonClick();
    void onSessionStateChanged();
    void onBuddyList();
    void onNewMessage(QString aMessage,QString aSender);
    void onSendButtonClick();
    void onError(QString aMessage);

private:
    Ui::Widget *mUi;
    ClientConnection *mConnection;
    QStringListModel mBuddyListModel;

};

#endif // WIDGET_H
