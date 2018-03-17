#ifndef CHATINPUTTEXT_H
#define CHATINPUTTEXT_H

#include <QTextEdit>
#include <QCompleter>

class ChatInputText : public QTextEdit
{
    Q_OBJECT

public:
    ChatInputText(QWidget *parent = 0);
    ~ChatInputText();

    void setCompleter(QCompleter *c);
    QCompleter *completer() const;
    QStringList autokeywords() const
    {
        return mAutoKeywords;
    }

protected:
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);

private slots:
    void insertCompletion(const QString &completion);

signals:
    void autocompleted(const QString &aText);

private:
    QString textUnderCursor() const;

private:
    QCompleter *c;
    QStringList mAutoKeywords;
};

#endif // CHATINPUTTEXT_H
