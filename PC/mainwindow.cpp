#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    if(consolebrowser) {
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
