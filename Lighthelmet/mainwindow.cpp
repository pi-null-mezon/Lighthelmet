#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileSystemModel>
#include <QStandardPaths>
#include <QMessageBox>
#include <QScreen>

#include <QFile>

#include "qsignalfileparser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qreal _dpi = QApplication::screens().at(0)->logicalDotsPerInch();
    qDebug("logicalDotsPerInch: %.1f", _dpi);
    QFont _font = font();
    _font.setPointSizeF( _dpi < 100.0 ? _font.pointSizeF() : _font.pointSizeF()*2.0);
    setFont(_font);

    setWindowTitle(APP_NAME);       
    setupFileSystemTreeView();
    loadSettings();
    setupHeadTB();
    setupFooterTB();

    connect(&qcolorgen,SIGNAL(colorGenerated(QColor)),ui->colorshowW,SLOT(updateColor(QColor)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_durationD_valueChanged(int value)
{
    ui->durationLCD->display(value);
}

void MainWindow::setupFooterTB()
{
    ui->footerTB->addAction(ui->actionPrevious);
    ui->footerTB->addSeparator();

    QWidget *_pspacerwidget = new QWidget;
    _pspacerwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->footerTB->addWidget(_pspacerwidget);

    ui->footerTB->addSeparator();
    ui->footerTB->addAction(ui->actionNext);
    ui->footerTB->setVisible(false);
}

void MainWindow::setupHeadTB()
{
    QWidget *_pspacerwidget = new QWidget;
    _pspacerwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->headTB->addWidget(_pspacerwidget);

    ui->headTB->addSeparator();
    ui->headTB->addAction(ui->actionExit);
}

void MainWindow::setupFileSystemTreeView()
{
    QFileSystemModel *model = new QFileSystemModel(this);
    model->setRootPath(QDir::currentPath());
    ui->treeView->setModel(model);

    QDir _dir(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
    _dir.mkdir(APP_NAME);
    _dir.cd(APP_NAME);

    ui->treeView->setRootIndex(model->index(_dir.absolutePath()));
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
}

void MainWindow::loadSettings()
{
    QDir _dir(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
    _dir.mkdir(APP_NAME);
    _dir.cd(APP_NAME);        

    QString _filename = _dir.absolutePath().append("/sineprobe.csv");
    if(QFileInfo(_filename).exists() == false) {
        QFile _file(":/Resources/sineprobe.csv");
        _file.copy(_filename);
        QFile::setPermissions(_filename,QFile::WriteOther);
    }

    settings = new QSettings(_dir.absolutePath().append("/%1.settings").arg(APP_NAME),QSettings::IniFormat,this);

    ui->signalnameL->setText(settings->value("Signal/Name",tr("Сигнал не выбран")).toString());
    signalfilename = settings->value("Signal/Filename","").toString();

    ui->durationD->setValue(settings->value("Signal/Duration",30).toInt());
}

void MainWindow::endSession()
{
    qcolorgen.stop();
    ui->colorshowW->setMode(QColorshowWidget::Void);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_selectsignalB_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_selectfileB_clicked()
{
    QList<QModelIndex> _indexeslist;
    QModelIndex _index = ui->treeView->currentIndex();
    while(_index.isValid()) {
        _indexeslist.push_front(_index);
        _index = _index.parent();
    }
    signalfilename.clear();
#ifdef Q_OS_LINUX
    signalfilename.append("/");
#endif
    for(int i = 0; i < _indexeslist.size()-1; ++i)
        signalfilename.append(QString("%1/").arg(_indexeslist.at(i).data().toString()));
    signalfilename.append(QString("%1").arg(_indexeslist.at(_indexeslist.size()-1).data().toString()));

    settings->setValue("Signal/Name",ui->treeView->currentIndex().data().toString());
    settings->setValue("Signal/Filename",signalfilename);

    ui->signalnameL->setText(ui->treeView->currentIndex().data().toString());
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_durationD_sliderReleased()
{
    settings->setValue("Signal/Duration",ui->durationD->value());
}

void MainWindow::on_runsessionB_clicked()
{
    auto _vparts = QSignalFileParser::parseSignalFromFile(signalfilename);
    if(_vparts.size() == 6) {
        qcolorgen.setParams(_vparts);
        ui->colorshowW->setMode(QColorshowWidget::Wait);
        ui->stackedWidget->setCurrentIndex(2);
        int _durationms = 1000; // it is just for visual effects, so it could be 0 if you want
        QTimer::singleShot(_durationms,ui->colorshowW,SLOT(enableShowMode()));
        QTimer::singleShot(_durationms,&qcolorgen,SLOT(start()));
        QTimer::singleShot(ui->durationLCD->value()*1000 + _durationms,this,SLOT(endSession()));
    } else {
        QMessageBox::warning(this,APP_NAME,tr("Файл %1 не существует или не соответсвует шаблону протокола!").arg(signalfilename),QMessageBox::Ok);
    }
}

void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    switch(arg1) {
        case 2:
            ui->headTB->setVisible(false);
            break;
        default:
            ui->headTB->setVisible(true);
            break;
    }
}
