#include "emotiselectorwidget.h"
#include <QPushButton>
#include <QGridLayout>
#include <QTextDocument>
#include <QPainter>
#include <QDebug>
#include <QHBoxLayout>
#include <QScrollBar>

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
    QStringList characters;
    characters<<"&#9786;"<<"&#9996;"<<"&#9787;"<<"&#9785;"<<"&#9757;"<<"&#9728;"<<"&#9731;"<<"&#9734;"
             <<"&#9788;"<<"&#9730;"<<"&#9729;"<<"&#9825;"<<"&#9835;"<<"&#9992;"<<"&#9742;";

    for(int j=0;j<characters.count();j++)
    {
        QPushButton *pButton=getHtmlButton(characters.at(j));
        loGrid2->addWidget(pButton);
        mContainer->addButton(pButton);
    }
    mScrollArea->setWidget(containerClient);
}

QPushButton *EmotiSelectorWidget::getHtmlButton(QString text)
{
    QPushButton *button=new QPushButton(this);

    button->setProperty("id",text);
    //p->setText("<p>I will display &#9788;</p>");
    QTextDocument Text;
    Text.setHtml(text);

    QFont f;
    f.setPointSize(20);
    Text.setDefaultFont(f);

    QPixmap pixmap(Text.size().width(), Text.size().height());
    pixmap.fill( Qt::transparent );
    QPainter painter( &pixmap );
    painter.setPen(QPen(QColor(255, 255, 0), 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
    Text.drawContents(&painter, pixmap.rect());

    QIcon ButtonIcon(pixmap);
    button->setIcon(ButtonIcon);
    button->setIconSize(pixmap.rect().size());
    return button;
}

void EmotiSelectorWidget::onButtonClicked(QAbstractButton *button)
{
    QString data=button->property("id").toString();
    data=data.simplified();
    emit emojiSelected(data);
}

void EmotiSelectorWidget::resizeEvent(QResizeEvent *event)
{
    containerClient->resize(size());
    mScrollArea->resize(size());
}
