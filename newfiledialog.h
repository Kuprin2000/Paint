#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIcon>
#include <QVBoxLayout>
#include <QDebug>

class NewFileDialog : public QDialog
{
private:
    QLabel main_label, width_label, height_label;
    QLineEdit width_edit, height_edit;
    QPushButton ok_button, exit_button;
    QVBoxLayout layout;

public:
    NewFileDialog(QWidget *parent = nullptr);
    int getWidth() const;
    int getHeight() const;
};

#endif // NEWFILEDIALOG_H
