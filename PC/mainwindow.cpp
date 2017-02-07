#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QEventLoop>
#include <QTimer>

//----------------------------------
QTextBrowser *consolebrowser = NULL;
//----------------------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("%1 v.%2").arg(APP_NAME, APP_VERSION));
    ui->statusBar->showMessage(QString("Based on Qt library, for the support contact %1").arg(APP_DESIGNER));

    consolebrowser = ui->textBrowser;

    __adjustactionstotoolbuttons();
    ui->searchportsAction->trigger();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::__adjustactionstotoolbuttons()
{
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->searchportsAction);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->selectfileAction);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->flashmcuAction);
    ui->toolBar->addSeparator();
}

void MainWindow::on_selectfileAction_triggered()
{
    QString _filename = QFileDialog::getOpenFileName(this, tr("Выберите файл"), "", tr("Signal (*.csv *txt)"));
    ui->filenameLE->setText(_filename);
}

void MainWindow::on_searchportsAction_triggered()
{
    ui->serialportCB->clear();
    ui->serialportCB->addItems(QSPProcessor::availablePorts());
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);

    if(consolebrowser) {
        switch(type) {
            case QtDebugMsg:
                consolebrowser->setTextColor(Qt::cyan);
                break;
            case QtInfoMsg:
                consolebrowser->setTextColor(Qt::gray);
                break;
            case QtWarningMsg:
                consolebrowser->setTextColor(Qt::red);
                break;
            case QtCriticalMsg:
                consolebrowser->setTextColor(Qt::red);
                break;
            case QtFatalMsg:
                consolebrowser->setTextColor(Qt::red);
                abort();
        }
        consolebrowser->append(msg);
    }
      /*QByteArray localMsg = msg.toLocal8Bit();
      switch (type) {
      case QtDebugMsg:
          fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          break;
      case QtInfoMsg:
          fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          break;
      case QtWarningMsg:
          fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          break;
      case QtCriticalMsg:
          fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          break;
      case QtFatalMsg:
          fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          abort();
      }*/
}

void MainWindow::on_pushButton_clicked()
{
    ui->selectfileAction->trigger();
}

void MainWindow::on_flashmcuAction_triggered()
{
    if(ui->serialportCB->count() == 0) {
        qWarning("Нет доступных портов!");
        return;
    }
    if(ui->filenameLE->text().isEmpty()) {
        qWarning("Не указан файл с сигналом!");
        return;
    }
    QSPProcessor _serialproc;
    _serialproc.openPort(ui->serialportCB->currentIndex());

    QSignalFileParser _parser;
    QByteArray _signal = _parser.parseFile(ui->filenameLE->text());

    /*qInfo("Содержимое файла:");
    for(int i = 0; i < _signal.size(); ++i) {
        qInfo("[%d]: %u", i, (uint8_t)_signal.at(i));
    }*/

    // Time delay, we should wait until Arduino will have finished booting
    QEventLoop _eloop;
    QTimer::singleShot(2000, &_eloop, SLOT(quit()));
    _eloop.exec();

    qInfo("Передача временного интервала на контроллер...");
    _serialproc.writeToPort(QByteArray("t10"));

    // Time delay, we should wait until Arduino will have finished booting
    QTimer::singleShot(200, &_eloop, SLOT(quit()));
    _eloop.exec();

    qInfo("Передача сигнала на контроллер...");
    _serialproc.writeToPort(_signal);

    // Time delay before connection will be closed, we should wait until Arduino will have finished booting
    QTimer::singleShot(15000, &_eloop, SLOT(quit()));
    _eloop.exec();
}
