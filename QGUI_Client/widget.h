#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>


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

private:
    Ui::Widget *mUi;
    ClientConnection *mConnection;

};

#endif // WIDGET_H
