#include "serverlogindialog.h"
#include "ui_serverlogindialog.h"
#include <QDebug>


ServerLoginDialog::ServerLoginDialog(QWidget *parent) :
    QDialog(parent),
    mUi(new Ui::ServerLoginDialog)
{
    setAttribute( Qt::WA_DeleteOnClose, true );
    mUi->setupUi(this);
    connect(mUi->okayButton,SIGNAL(clicked(bool)),this,SLOT(onOkayButtonClick()));
    mUi->searchButton->setVisible(false);
}

void ServerLoginDialog::onOkayButtonClick()
{
    emit setServerData(mUi->ipLineEdit->text(),mUi->portLineEdit->text(),mUi->avatarName->text());
    close();
}

ServerLoginDialog::~ServerLoginDialog()
{
    delete mUi;
}
