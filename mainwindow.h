#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QColorDialog>
#include <QFontDialog>
#include <QFileDialog>
#include <QStatusBar>
#include <QApplication>
#include <QClipboard>
#include "mygraphicsview.h"
#include "brushsettingsdialog.h"
#include "newfiledialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QPen my_pen;
    MyGraphicsView my_graphics_view;
    QImage *my_image;
    QMenuBar main_menu;
    QToolBar my_tools;
    QMenu file_menu;
    QLabel current_color;
    QVBoxLayout my_layout;
    QWidget main_widget;
    QFont my_font;
    QStatusBar status_bar;
    int current_action_flag, image_height, image_width;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void newFileSlot();
    void openFileSlot();
    void saveFileSlot();
    void canselActionSlot();
    void exitSlot();
    void brushSlot();
    void eraserSlot();
    void lineSlot();
    void flowSlot();
    void pipetteSlot();
    void textSlot();
    void ellipseSlot();
    void rectSlot();
    void roundedRectSlot();
    void paleteSlot();
    void brushSettingsSlot();
    void textSettingsSlot();
    void colorChangedSlot();
    void pasteSlot();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
