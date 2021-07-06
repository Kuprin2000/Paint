#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QIcon>
#include <QPushButton>
#include <QVBoxLayout>

class TextDialog : public QDialog
{
    Q_OBJECT

private:
    QLabel label;
    QLineEdit edit;
    QPushButton ok_button, exit_button;
    QVBoxLayout layout;

public:
    TextDialog(QWidget *parent = nullptr);
    QString getText() const;
};

#endif // TEXTDIALOG_H
