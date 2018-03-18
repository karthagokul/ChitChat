#include "emotiselectorwidget.h"
#include <QPushButton>
#include <QGridLayout>
#include <QTextDocument>
#include <QPainter>
#include <QDebug>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QDirIterator>
#include <QToolButton>

EmotiSelectorWidget::EmotiSelectorWidget(QWidget *parent) :
    QWidget(parent)
{
    mContainer=new QButtonGroup(this);
    connect(mContainer,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(onButtonClicked(QAbstractButton*)));
    containerClient= new QWidget(this); //creates a new widget which has "this" as its parent, meaning it will be deleted if "this" is destroyed
    QGridLayout *loGrid2 = new QGridLayout(containerClient);
    setStyleSheet("background-color:white;");
    mScrollArea=new QScrollArea(this);
    mScrollArea->setGeometry( geometry() );
    mScrollArea->setWidgetResizable( false );
    containerClient->setLayout(loGrid2);
    loGrid2->setSpacing(0);
    loGrid2->setMargin(0);
    QDirIterator it(":/emoticons/", QDirIterator::Subdirectories);
    int i=0;
    int j=1;
    while (it.hasNext()) {
        QString file=it.next();
        if(file.endsWith(".png"))//We only supports png
        {
            QToolButton *pButton=new QToolButton(this);
            pButton->setFixedSize(QSize(32,32));
            pButton->setProperty("id",file);
            QString bgImage=QString("border-image:url(")+file+QString(");");
            pButton->setStyleSheet(bgImage);
            loGrid2->addWidget(pButton,j,i);
            i++;
            if(i==8)
            {
                i=0;
                j++;
            }
            mContainer->addButton(pButton);
        }
    }

    mScrollArea->setWidget(containerClient);
}


void EmotiSelectorWidget::onButtonClicked(QAbstractButton *button)
{
    qDebug()<<"Clicked";
    QString data=button->property("id").toString();
    data=data.simplified();
    emit emojiSelected(data);
}

void EmotiSelectorWidget::resizeEvent(QResizeEvent *event)
{
   // containerClient->resize(size());
    mScrollArea->resize(size());
}
