#include "textdialog.h"

TextDialog::TextDialog(QWidget *parent): QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    setWindowTitle("Добавить надпись");
    setWindowIcon(QIcon(":/images/logo.png"));
    setFixedSize(300, 150);

    QFont tmp_font = label.font();
    tmp_font.setPixelSize(15);
    setFont(tmp_font);

    label.setText("Введите текст надписи");
    ok_button.setText("ОК");
    exit_button.setText("Отмена");

    layout.addWidget(&label);
    layout.addWidget(&edit);
    layout.addWidget(&ok_button);
    layout.addWidget(&exit_button);
    setLayout(&layout);

    connect(&ok_button, SIGNAL(clicked()), SLOT(accept()));
    connect(&exit_button, SIGNAL(clicked()), SLOT(reject()));
}

QString TextDialog::getText() const
{
    return edit.text();
}
