#include "emotiselectorwidget.h"
#include <QPushButton>
#include <QGridLayout>
#include <QTextDocument>
#include <QPainter>
#include <QDebug>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QDirIterator>

EmotiSelectorWidget::EmotiSelectorWidget(QWidget *parent) :
    QWidget(parent)
{
    mContainer=new QButtonGroup(this);
    connect(mContainer,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(onButtonClicked(QAbstractButton*)));
    containerClient= new QWidget(this); //creates a new widget which has "this" as its parent, meaning it will be deleted if "this" is destroyed
    QHBoxLayout *loGrid2 = new QHBoxLayout(containerClient);
    mScrollArea=new QScrollArea(this);
   // mScrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
   // mScrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    mScrollArea->setGeometry( geometry() );

    mScrollArea->setWidgetResizable( true );
    //mScrollArea->horizontalScrollBar()->setStyleSheet();

    containerClient->setLayout(loGrid2);

    QDirIterator it(":/emoticons/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString file=it.next();
        if(file.endsWith(".png"))//We only supports png
        {
            QPushButton *pButton=new QPushButton(this);
            pButton->setStyleSheet("background-color:transparent");
            pButton->setProperty("id",file);
            pButton->setIcon(QIcon(file));
            loGrid2->addWidget(pButton);
            mContainer->addButton(pButton);
            //qDebug() << it.next();
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
    containerClient->resize(size());
    mScrollArea->resize(size());
}
