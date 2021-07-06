#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QScrollBar>
#include <QQueue>
#include <QVector>
#include <QStack>
#include <cmath>
#include <QString>
#include <QApplication>
#include <QClipboard>
#include <QTimer>
#include "textdialog.h"

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT

private:
    QGraphicsScene my_scene;
    QPen *my_pen, white_pen;
    QImage *my_image;
    int *my_action_flag;
    QPainter my_painter;
    QPointF curren_point, previous_point, figure_start_point;
    QFont *my_font;
    bool draw_line_flag, draw_figure_flag, need_to_save_flag, need_to_refresh_flag;
    int *image_heigth, *image_width;
    QStack<QString> dumps_stack;
    QTimer my_timer;
    QImage tmp_image;
    void flow(QColor &start_colot, QColor &result_color, QPoint start_point);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

public:
    MyGraphicsView(QWidget *parent = nullptr);
    void setPen(QPen &input_pen);
    void setQImage(QImage &input_image);
    void setImageHeigth(int &input_height);
    void setImageWidth (int &input_width);
    void setActionFlag(int &input_flag);
    void setFont(QFont &input_font);
    void save(QString path);
    bool getNeedToSaveFlag() const;
    void setNeedToSaveFlag(bool input);
    void loadFile(QString path);
    void loadDump(QString name);
    void clearDumps();
    QString getTopDump();
    int dumpCounter() const;

public slots:
    void refreshSlot();

signals:
    void colorChangedSignal();
    void canselSignal();
    void pasteSignal();
};

#endif // MYGRAPHICSVIEW_H
