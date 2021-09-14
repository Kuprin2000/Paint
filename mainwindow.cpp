#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    setWindowTitle("Paint - Файл без названия");
    setWindowIcon(QIcon(":/images/logo.png"));
    setMinimumSize(600, 600);

    my_tools.setMovable(false);

    file_menu.addAction("Создать", this, SLOT(newFileSlot()), Qt::CTRL + Qt::Key_N);
    file_menu.addAction("Открыть", this, SLOT(openFileSlot()), Qt::CTRL + Qt::Key_O);
    file_menu.addAction("Сохранить", this, SLOT(saveFileSlot()), Qt::CTRL + Qt::Key_S);
    file_menu.setTitle("Файл");

    main_menu.addMenu(&file_menu);
    main_menu.addAction("Настройка кисти", this, SLOT(brushSettingsSlot()));
    main_menu.addAction("Настройка текста", this, SLOT(textSettingsSlot()));
    main_menu.addAction("Отменить действие", this, SLOT(canselActionSlot()));
    main_menu.addAction("Выход", this, SLOT(exitSlot()));

    setMenuBar(&main_menu);

    my_tools.setContextMenuPolicy(Qt::PreventContextMenu);
    my_tools.addAction(QIcon(":/images/brush.png"), "кисть", this, SLOT(brushSlot()));
    my_tools.addAction(QIcon(":/images/eraser.png"), "ластик", this, SLOT(eraserSlot()));
    my_tools.addAction(QIcon(":/images/line.png"), "линия", this, SLOT(lineSlot()));
    my_tools.addAction(QIcon(":/images/flow.png"), "заливка", this, SLOT(flowSlot()));
    my_tools.addAction(QIcon(":/images/pipette.png"), "пипетка", this, SLOT(pipetteSlot()));
    my_tools.addAction(QIcon(":/images/text.png"), "текст", this, SLOT(textSlot()));
    my_tools.addAction(QIcon(":/images/ellipse.png"), "эллипс", this, SLOT(ellipseSlot()));
    my_tools.addAction(QIcon(":/images/rect.png"), "прямоугольник", this, SLOT(rectSlot()));
    my_tools.addAction(QIcon(":/images/rounded_rect.png"), "прямоугольник с закругленными углами", this, SLOT(roundedRectSlot()));
    my_tools.addAction(QIcon(":/images/palete.png"), "палитра", this, SLOT(paleteSlot()));
    my_tools.addSeparator();
    my_tools.addWidget(&current_color);

    addToolBar(Qt::TopToolBarArea, &my_tools);

    image_height = 800;
    image_width = 1200;
    my_pen.setBrush(Qt::SolidPattern);
    my_pen.setColor(Qt::black);
    my_pen.setStyle(Qt::SolidLine);
    my_pen.setWidth(10);
    my_pen.setCapStyle(Qt::RoundCap);
    my_font = QFont("Times", 20, QFont::Normal);
    current_color.setAutoFillBackground(true);
    current_color.setPalette(QPalette(Qt::black));
    current_color.setMinimumSize(50, 50);

    my_image = new QImage(image_width, image_height, QImage::Format_ARGB32_Premultiplied);
    my_image->fill(Qt::white);
    my_graphics_view.setPen(my_pen);
    my_graphics_view.setQImage(*my_image);
    my_graphics_view.setActionFlag(current_action_flag);
    my_graphics_view.setFont(my_font);
    my_graphics_view.setImageWidth(image_width);
    my_graphics_view.setImageHeigth(image_height);

    current_action_flag = 0;

    setCentralWidget(&my_graphics_view);
    setStatusBar(&status_bar);

    connect(&my_graphics_view, SIGNAL(colorChangedSignal()), SLOT(colorChangedSlot()));
    connect(&my_graphics_view, SIGNAL(canselSignal()), SLOT(canselActionSlot()));
    connect(&my_graphics_view, SIGNAL(pasteSignal()), SLOT(pasteSlot()));
}

MainWindow::~MainWindow()
{
    delete my_image;
}

void MainWindow::newFileSlot()
{
    setWindowTitle("Paint - Файл без названия");

    if (my_graphics_view.getNeedToSaveFlag() == 1)
    {
        QFileDialog *dialog = new QFileDialog;
        QString path = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "*.png ;; *.jpg");

        delete dialog;

        my_graphics_view.save(path);
    }

    NewFileDialog *dialog = new NewFileDialog;

    if (dialog->exec()== QDialog::Accepted)
    {
        setWindowTitle("Paint - Файл без названия");

        image_width = dialog->getWidth();
        image_height=dialog->getHeight();

        if(image_width < 1) image_width=100;
        if(image_height <1) image_height=100;
        if(image_width>3000) image_width=3000;
        if(image_height>3000) image_height=3000;

        delete dialog;
        delete my_image;

        current_action_flag = 0;

        status_bar.clearMessage();

        my_image = new QImage(image_width, image_height, QImage::Format_ARGB32_Premultiplied);
        my_image->fill(Qt::white);
        my_graphics_view.setQImage(*my_image);
        my_graphics_view.clearDumps();
    }
}

void MainWindow::openFileSlot()
{
    if (my_graphics_view.getNeedToSaveFlag() == 1)
    {
        QFileDialog *dialog = new QFileDialog;
        QString path = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "*.png ;; *.jpg");

        delete dialog;

        my_graphics_view.save(path);
    }

    QString path = QFileDialog::getOpenFileName(this, "Сохранить файл", "", "*.png ;; *.jpg");
    QStringList tmp_list = path.split("/");

    setWindowTitle("Paint - " + tmp_list.last());

    QImage tmp_image;
    tmp_image.load(path);

    image_width=tmp_image.width();
    image_height=tmp_image.height();

    delete my_image;

    my_image = new QImage(image_width, image_height, QImage::Format_ARGB32_Premultiplied);
    my_image->fill(Qt::white);
    my_graphics_view.setQImage(*my_image);
    my_graphics_view.loadFile(path);

    current_action_flag = 0;

    status_bar.clearMessage();
}

void MainWindow::saveFileSlot()
{
    QFileDialog *dialog = new QFileDialog;
    QString path = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "*.png ;; *.jpg");

    QStringList tmp_list = path.split("/");

    setWindowTitle("Paint - " + tmp_list.last());

    delete dialog;

    my_graphics_view.save(path);
}

void MainWindow::canselActionSlot()
{
    if(my_graphics_view.dumpCounter()!=0)
    {
        my_graphics_view.loadDump(my_graphics_view.getTopDump());
    }
}

void MainWindow::exitSlot()
{
    if (my_graphics_view.getNeedToSaveFlag() == 1)
    {
        QFileDialog *dialog = new QFileDialog;
        QString path = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "*.png ;; *.jpg");
        delete dialog;
        my_graphics_view.save(path);
    }

    QFile file;

    for (int i=0; i<11; ++i)
    {
        file.setFileName(QString::number(i) + ".ppm");
        file.remove();
    }

    file.setFileName("tmp.ppm");
    file.remove();
    close();

    exit(0);
}

void MainWindow::brushSlot()
{
    current_action_flag = 1;
    status_bar.clearMessage();
    status_bar.showMessage("Кисть");
}

void MainWindow::eraserSlot()
{
    current_action_flag = 2;
    status_bar.clearMessage();
    status_bar.showMessage("Ластик");
}

void MainWindow::lineSlot()
{
    current_action_flag = 3;
    status_bar.clearMessage();
    status_bar.showMessage("Линия");
}

void MainWindow::flowSlot()
{
    current_action_flag = 4;
    status_bar.clearMessage();
    status_bar.showMessage("Заливка");
}

void MainWindow::pipetteSlot()
{
    current_action_flag =5;
    status_bar.clearMessage();
    status_bar.showMessage("Пипетка");
}

void MainWindow::textSlot()
{
    current_action_flag =6;
    status_bar.clearMessage();
    status_bar.showMessage("Текст");
}

void MainWindow::ellipseSlot()
{
    current_action_flag = 7;
    status_bar.clearMessage();
    status_bar.showMessage("Эллипс");
}

void MainWindow::rectSlot()
{
    current_action_flag = 8;
    status_bar.clearMessage();
    status_bar.showMessage("Прямоугольник");
}

void MainWindow::roundedRectSlot()
{
    current_action_flag = 9;
    status_bar.clearMessage();
    status_bar.showMessage("Прямоугольник с закругленными углами");
}

void MainWindow::paleteSlot()
{
    QColorDialog *dialog = new QColorDialog(this);
    dialog->setWindowTitle("Палитра");
    dialog->setWindowIcon(QIcon(":/images/logo.png"));

    QColor tmp_color = dialog->getColor();
    my_pen.setColor(tmp_color);
    current_color.setPalette(my_pen.color());
    my_graphics_view.setNeedToSaveFlag(true);

    delete dialog;
}

void MainWindow::brushSettingsSlot()
{
    BrushSettingsDialog *dialog = new BrushSettingsDialog;
    dialog->setCurrentWidth(my_pen.width());

    int thikness=0, style_flag=0;
    if (dialog->exec() == QDialog::Accepted)
    {
        thikness = dialog->getThikness();
        style_flag = dialog->getStyle();
    }

    delete dialog;

    if (thikness>=0 && thikness<100) my_pen.setWidth(thikness);

    if (thikness>100) my_pen.setWidth(100);

    switch (style_flag)
    {
    case 1: my_pen.setStyle(Qt::SolidLine); break;
    case 2:  my_pen.setStyle(Qt::DashLine); break;
    case 3: my_pen.setStyle(Qt::DotLine); break;
    case 4: my_pen.setStyle(Qt::DashDotLine); break;
    }
}

void MainWindow::textSettingsSlot()
{
    QFontDialog *dialog = new QFontDialog(this);

    if (dialog->exec()== QDialog::Accepted)
    {
        my_font =dialog->selectedFont();
    }

    delete dialog;
}



void MainWindow::colorChangedSlot()
{
    current_color.setPalette(my_pen.color());
}

void MainWindow::pasteSlot()
{
    setWindowTitle("Paint - Файл без названия");

    QClipboard *tmp_clipboard = QApplication::clipboard();
    QImage new_image = tmp_clipboard->image();

    if(!new_image.isNull())
    {
        if (my_graphics_view.getNeedToSaveFlag() == 1)
        {
            QFileDialog *dialog = new QFileDialog;
            QString path = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "*.png ;; *.jpg");

            delete dialog;

            my_graphics_view.save(path);
        }

        image_width=new_image.width();
        image_height=new_image.height();

        delete my_image;
        my_image = new QImage(new_image);
        my_graphics_view.setQImage(*my_image);

        current_action_flag = 0;

        status_bar.clearMessage();

        my_graphics_view.clearDumps();
        my_graphics_view.setNeedToSaveFlag(1);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (my_graphics_view.getNeedToSaveFlag() == 1)
    {
        QFileDialog *dialog = new QFileDialog;
        QString path = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "*.png ;; *.jpg");

        delete dialog;

        my_graphics_view.save(path);
    }

    QFile file;

    for (int i=0; i<11; ++i)
    {
        file.setFileName(QString::number(i) + ".ppm");
        file.remove();
    }

    file.setFileName("tmp.ppm");
    file.remove();

    close();

    exit(0);
}
