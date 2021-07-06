#include "mygraphicsview.h"

void MyGraphicsView::flow(QColor &start_color, QColor &result_color, QPoint start_point)
{
    QQueue<QPoint> queue;

    if(my_image->pixelColor(start_point) == start_color && my_image->pixelColor(start_point) != result_color)
    {
        queue.append(start_point);
        auto iter = queue.begin();

        while (queue.isEmpty()!=true)
        {
            if(my_image->pixelColor(*iter) == start_color)
            {
                QPoint west = *iter, east = *iter;

                while (west.x()>0 && my_image->pixelColor(west)==start_color)
                {
                    west.setX(west.x()-1);
                }

                while (east.x() < *image_width && my_image->pixelColor(east)==start_color)
                {
                    east.setX(east.x()+1);
                }

                int start_x = west.x(), end_x = east.x(), y = west.y();

                while (start_x!=end_x)
                {
                    my_image->setPixelColor(start_x, y, result_color);

                    if (y>0)
                    {
                        if (my_image->pixelColor(start_x, y-1) == start_color)
                        {
                            queue.append(QPoint(start_x, y-1)) ;
                        }
                    }

                    if (y<*image_heigth-1)
                    {
                        if (my_image->pixelColor(start_x, y+1) == start_color)
                        {
                            queue.append(QPoint(start_x, y+1)) ;
                        }
                    }

                    ++start_x;
                }
            }

            iter = queue.begin();
            queue.erase(iter);
            iter = queue.begin();
        }
    }

    need_to_refresh_flag =1;
}

MyGraphicsView::MyGraphicsView(QWidget *parent): QGraphicsView(parent)
{
    setScene(&my_scene);

    setBackgroundBrush(QBrush(Qt::gray));

    draw_line_flag = 0;
    draw_figure_flag=0;
    need_to_save_flag = 0;

    connect(&my_timer, SIGNAL(timeout()), SLOT(refreshSlot()));

    my_timer.start(40);

    need_to_refresh_flag = 1;

    white_pen.setColor(Qt::white);
}

void MyGraphicsView::setPen(QPen &input_pen)
{
    my_pen = &input_pen;
}

void MyGraphicsView::setQImage(QImage &input_image)
{
    clearDumps();

    my_image = &input_image;

    my_scene.clear();
    my_scene.addPixmap(QPixmap::fromImage(*my_image));

    setAlignment(Qt::AlignLeft | Qt::AlignTop);

    need_to_refresh_flag =1;
}

void MyGraphicsView::setImageHeigth(int &input_height)
{
    clearDumps();
    image_heigth = &input_height;
}

void MyGraphicsView::setImageWidth(int &input_width)
{
    clearDumps();
    image_width = &input_width;
}

void MyGraphicsView::setActionFlag(int &input_flag)
{
    my_action_flag = &input_flag;
}

void MyGraphicsView::setFont(QFont &input_font)
{
    my_font = &input_font;
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (*my_action_flag!=0)
    {
        QString dump_name;

        if(dumps_stack.size() >10)
        {
            QStack<QString>::iterator iter = dumps_stack.begin();
            dump_name=*iter;
            dumps_stack.erase(iter);
        }

        else
        {
            dump_name = QString::number(dumps_stack.size()) + ".ppm";
        }

        dumps_stack.push(dump_name);
        my_image->save(dump_name);

        need_to_save_flag =1;

        curren_point.setX(event->x()+this->horizontalScrollBar()->value());
        curren_point.setY(event->y()+this->verticalScrollBar()->value());

        my_painter.begin(my_image);
        my_painter.setPen(*my_pen);

        if (*my_action_flag == 1)
        {
            my_painter.drawPoint(curren_point);
            my_scene.clear();

            previous_point = curren_point;
        }

        if (*my_action_flag == 2)
        {
            QPen white_pen = *my_pen;
            white_pen.setWidth(my_pen->width());
            white_pen.setColor(Qt::white);

            my_painter.setPen(white_pen);
            my_painter.drawPoint(curren_point);

            my_scene.clear();

            previous_point = curren_point;

            my_painter.setPen(*my_pen);
        }

        if (*my_action_flag == 3)
        {
            if (draw_line_flag == 0)
            {
                draw_line_flag = 1;

                previous_point = curren_point;
            }

            else
            {
                my_painter.drawLine(previous_point, curren_point);

                draw_line_flag = 0;
            }
        }

        if (*my_action_flag == 4)
        {
            QColor start_color = my_image->pixelColor(curren_point.x(), curren_point.y()), result_color = my_pen->color();
            flow(start_color, result_color, QPoint(curren_point.x(), curren_point.y()));

        }

        if (*my_action_flag == 5)
        {
            my_pen->setColor(my_image->pixelColor(curren_point.x(), curren_point.y()));
            emit colorChangedSignal();
        }

        if (*my_action_flag == 6)
        {
            QString text;
            TextDialog *dialog = new TextDialog;

            if (dialog->exec() == QDialog::Accepted)
            {
                text=dialog->getText();
            }

            delete dialog;

            my_painter.setFont(*my_font);
            my_painter.drawText(curren_point, text);

        }

        if (*my_action_flag == 7 && draw_figure_flag == 0)
        {
            my_image->save("tmp.ppm");
            draw_figure_flag =1;
            figure_start_point = curren_point;
            my_painter.drawEllipse(curren_point, 30, 30);

        }

        if (*my_action_flag == 8 && draw_figure_flag == 0)
        {
            my_image->save("tmp.ppm");
            draw_figure_flag =1;
            figure_start_point = curren_point;
            my_painter.drawRect(curren_point.x(), curren_point.y(), 30, 30);

        }

        if (*my_action_flag == 9 && draw_figure_flag == 0)
        {
            my_image->save("tmp.ppm");
            draw_figure_flag =1;
            figure_start_point = curren_point;
            my_painter.drawRoundedRect(curren_point.x(), curren_point.y(), 30, 30, 6, 6);

        }

        my_painter.end();

        need_to_refresh_flag =1;
    }
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (*my_action_flag!=0)
    {
        need_to_save_flag =1;

        curren_point.setX(event->x()+this->horizontalScrollBar()->value());
        curren_point.setY(event->y()+this->verticalScrollBar()->value());

        my_painter.begin(my_image);
        my_painter.setPen(*my_pen);

        if (*my_action_flag == 1)
        {
            my_painter.drawLine(previous_point, curren_point);
        }

        if(*my_action_flag == 2)
        {
            white_pen.setWidth(my_pen->width());
            my_painter.setPen(white_pen);
            my_painter.drawLine(previous_point, curren_point);
            my_painter.setPen(*my_pen);
        }

        if(*my_action_flag == 7 && draw_figure_flag == 1)
        {
            tmp_image.load("tmp.ppm");
            my_painter.drawImage(QPoint(0, 0), tmp_image);
            int delta_x = curren_point.x() - figure_start_point.x(), delta_y = curren_point.y() - figure_start_point.y();
            my_painter.drawEllipse(figure_start_point, delta_x, delta_y);
        }

        if(*my_action_flag == 8 && draw_figure_flag == 1)
        {
            tmp_image.load("tmp.ppm");
            my_painter.drawImage(QPoint(0, 0), tmp_image);
            int delta_x = curren_point.x() - figure_start_point.x(), delta_y = curren_point.y() - figure_start_point.y();
            my_painter.drawRect(figure_start_point.x(), figure_start_point.y(), delta_x, delta_y);
        }

        if(*my_action_flag == 9 && draw_figure_flag == 1)
        {
            tmp_image.load("tmp.ppm");
            my_painter.drawImage(QPoint(0, 0), tmp_image);
            int delta_x = curren_point.x() - figure_start_point.x(), delta_y = curren_point.y() - figure_start_point.y();
            my_painter.drawRoundedRect(figure_start_point.x(), figure_start_point.y(), delta_x, delta_y, abs(delta_x)/5, abs(delta_y)/5);
        }

        previous_point = curren_point;

        my_painter.end();
    }

    need_to_refresh_flag =1;
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(*my_action_flag == 7 && draw_figure_flag == 1)
    {
        draw_figure_flag = 0;
    }

    if(*my_action_flag == 8 && draw_figure_flag == 1)
    {
        draw_figure_flag = 0;
    }

    if(*my_action_flag == 9 && draw_figure_flag == 1)
    {
        draw_figure_flag = 0;
    }

    need_to_refresh_flag =1;
    need_to_save_flag =1;
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Z && event->modifiers() == Qt::CTRL)
    {
        emit canselSignal();
    }

    if (event->key() == Qt::Key_V && event->modifiers() == Qt::CTRL)
    {
        emit pasteSignal();
    }

    if (event->key() == Qt::Key_C && event->modifiers() == Qt::CTRL)
    {
        QApplication::clipboard()->setPixmap(QPixmap::fromImage(*my_image));
    }

    need_to_refresh_flag =1;
}

void MyGraphicsView::save(QString path)
{
    my_image->save(path);

    need_to_save_flag=0;
    need_to_refresh_flag =1;
}

bool MyGraphicsView::getNeedToSaveFlag() const
{
    return need_to_save_flag;
}

void MyGraphicsView::setNeedToSaveFlag(bool input)
{
    need_to_save_flag = input;
}

void MyGraphicsView::loadFile(QString path)
{
    clearDumps();

    my_painter.begin(my_image);

    QImage tmp_image;
    tmp_image.load(path);

    my_painter.drawImage(QPoint(0, 0), tmp_image);
    my_painter.end();

    need_to_refresh_flag =1;
}

void MyGraphicsView::loadDump(QString name)
{
    my_painter.begin(my_image);

    QImage tmp_image;
    tmp_image.load(name);

    my_painter.drawImage(QPoint(0, 0), tmp_image);
    my_painter.end();

    need_to_refresh_flag =1;
}

void MyGraphicsView::clearDumps()
{
    dumps_stack.clear();
}

QString MyGraphicsView::getTopDump()
{
    return dumps_stack.pop();
}

int MyGraphicsView::dumpCounter() const
{
    return dumps_stack.size();
}

void MyGraphicsView::refreshSlot()
{
    if (need_to_refresh_flag ==1)
    {
        my_scene.clear();
        my_scene.addPixmap(QPixmap::fromImage(*my_image));

        setAlignment(Qt::AlignLeft | Qt::AlignTop);

        need_to_refresh_flag=0;
    }
}
