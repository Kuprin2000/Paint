#ifndef BRUSHSETTINGSDIALOG_H
#define BRUSHSETTINGSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QSlider>

class BrushSettingsDialog : public QDialog
{
    Q_OBJECT

private:
    QLabel style_label, width_label, current_width_label;
    QSlider *width_slider;
    QRadioButton solid_button, dash_button, dot_button, dashdot_button;
    QPushButton ok_button, exit_button;
    QVBoxLayout layout;
    int current_width;

public:
    BrushSettingsDialog(QWidget *parent = nullptr);
    int getStyle() const;
    int getThikness() const;
    void setCurrentWidth(int input);

public slots:
    void valueChangedSlot(int input);
};

#endif // BRUSHSETTINGSDIALOG_H
