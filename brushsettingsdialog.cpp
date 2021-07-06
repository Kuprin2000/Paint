#include "brushsettingsdialog.h"

BrushSettingsDialog::BrushSettingsDialog(QWidget *parent): QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    setWindowTitle("Настройка кисти");
    setWindowIcon(QIcon(":/images/logo.png"));
    setFixedSize(300, 300);

    QFont tmp_font = style_label.font();
    tmp_font.setPixelSize(15);
    setFont(tmp_font);
    style_label.setText("Контуры фигур");
    width_label.setText("Толщина кисти");
    solid_button.setText("Сплошные");
    dash_button.setText("Пунктирные");
    dashdot_button.setText("Штрихпунктирные");
    dot_button.setText("Линии из точек");
    ok_button.setText("OK");
    exit_button.setText("Отмена");

    solid_button.setChecked(true);
    current_width=20;
    width_slider = new QSlider(Qt::Horizontal, this);
    width_slider->setRange(1, 100);
    //width_slider.setTickInterval(1);
    width_slider->setValue(20);
    current_width_label.setText("20");
    layout.addWidget(&style_label);
    layout.addWidget(&solid_button);
    layout.addWidget(&dash_button);
    layout.addWidget(&dot_button);
    layout.addWidget(&dashdot_button);
    layout.addWidget(&width_label);
    layout.addWidget(&current_width_label);
    layout.addWidget(width_slider);
    layout.addWidget(&ok_button);
    layout.addWidget(&exit_button);
    connect(&ok_button, SIGNAL(clicked()), SLOT(accept()));
    connect(&exit_button, SIGNAL(clicked()), SLOT(reject()));
    connect(width_slider, SIGNAL(valueChanged(int)), SLOT(valueChangedSlot(int)));
    setLayout(&layout);
}

int BrushSettingsDialog::getStyle() const
{
    if(solid_button.isChecked() == true) return 1;
    if(dash_button.isChecked() == true) return 2;
    if(dot_button.isChecked() == true) return 3;
    if(dashdot_button.isChecked() == true) return 4;
    return 0;
}

int BrushSettingsDialog::getThikness() const
{
    return current_width;
}

void BrushSettingsDialog::setCurrentWidth(int input)
{
    current_width = input;
    width_slider->setValue(input);
    current_width_label.setText(QString::number(input));
}

void BrushSettingsDialog::valueChangedSlot(int input)
{
    current_width = input;
    current_width_label.setText(QString::number(current_width));
}
