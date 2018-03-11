#ifndef SERVERLOGINDIALOG_H
#define SERVERLOGINDIALOG_H

#include <QDialog>

namespace Ui {
class ServerLoginDialog;
}

#ifdef SEARCH_SERVER
class DiscoveryManager;
#endif

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

#ifdef SEARCH_SERVER
    void onServerInfo(QString ip,int port);
    void onSearchClick();
#endif

private:
    Ui::ServerLoginDialog *mUi;
#ifdef SEARCH_SERVER
    DiscoveryManager *mSearch;
#endif
};

#endif // SERVERLOGINDIALOG_H
