#ifndef EMOTISELECTORWIDGET_H
#define EMOTISELECTORWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include <QScrollArea>


class QPushButton;
class EmotiSelectorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmotiSelectorWidget(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event);
    QPushButton *getHtmlButton(QString text);
signals:
    void emojiSelected(const QString &text);
private slots:
    void onButtonClicked(QAbstractButton *button);
private:
    QButtonGroup *mContainer;
    QScrollArea *mScrollArea;
};

#endif // EMOTISELECTORWIDGET_H
