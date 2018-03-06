#ifndef SERVERLOGINDIALOG_H
#define SERVERLOGINDIALOG_H

#include <QDialog>

namespace Ui {
class ServerLoginDialog;
}

class ServerLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServerLoginDialog(QWidget *parent = 0);
    ~ServerLoginDialog();

signals:
    void setServerData(QString aHostIp ,QString aPort,QString aAvatarName);
protected slots:
    void onOkayButtonClick();

private:
    Ui::ServerLoginDialog *mUi;
};

#endif // SERVERLOGINDIALOG_H
