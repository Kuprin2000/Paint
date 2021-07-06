#include "newfiledialog.h"

NewFileDialog::NewFileDialog(QWidget *parent): QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    setWindowTitle("Новый файл");
    setWindowIcon(QIcon(":/images/logo.png"));
    setFixedSize(300, 250);

    QFont tmp_font = height_label.font();
    tmp_font.setPixelSize(15);
    setFont(tmp_font);

    main_label.setText("Введите информацию для создания нового файла");
    main_label.setWordWrap(true);
    width_label.setText("Ширина полотна");
    height_label.setText("Высота полотна");
    ok_button.setText("OK");
    exit_button.setText("Отмена");

    layout.addWidget(&main_label);
    layout.addWidget(&width_label);
    layout.addWidget(&width_edit);
    layout.addWidget(&height_label);
    layout.addWidget(&height_edit);
    layout.addWidget(&ok_button);
    layout.addWidget(&exit_button);
    setLayout(&layout);

    connect(&ok_button, SIGNAL(clicked()), SLOT(accept()));
    connect(&exit_button, SIGNAL(clicked()), SLOT(reject()));
}

int NewFileDialog::getWidth() const
{
    return width_edit.text().toInt();
}

int NewFileDialog::getHeight() const
{
    return height_edit.text().toInt();
}
