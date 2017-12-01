#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QEventLoop>
#include <QLabel>
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
    ui->statusBar->showMessage(QString("Open source project").arg(APP_WEB));

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
    ui->toolBar->addAction(ui->helpAction);
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
                consolebrowser->setTextColor(QColor(127,127,255));
                break;
            case QtInfoMsg:
                consolebrowser->setTextColor(Qt::lightGray);
                break;
            case QtWarningMsg:
                consolebrowser->setTextColor(QColor(255,127,0));
                break;
            case QtCriticalMsg:
                consolebrowser->setTextColor(QColor(255,127,0));
                break;
            case QtFatalMsg:
                consolebrowser->setTextColor(QColor(255,127,0));
                abort();
        }
        consolebrowser->append(msg);
    }
      /*QByteArray localMsg = msg.toUtf8();
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
    // First let's check all start conditions
    if(ui->serialportCB->count() == 0) {
        qWarning("Нет доступных портов!");
        return;
    }
    if(ui->filenameLE->text().isEmpty()) {
        qWarning("Не указан файл с сигналом!");
        return;
    }

    // As I use local instances of the classes I will also use here a QEventLoop
    // to make all necessary time delays and process all signals and messages
    QSPProcessor _serialproc;
    _serialproc.openPort(ui->serialportCB->currentIndex());

    QSignalFileParser _parser;
    std::vector<QByteArray> _vmsgs = _parser.parseSignalFromFile(ui->filenameLE->text());

    for(size_t j = 0; j < _vmsgs.size(); ++j) {
        qDebug("----------\nСообщение %u", j);
        QByteArray _msg = _vmsgs[j];
        for(int i = 0; i < _msg.size(); ++i) {
            qDebug("<%d>: %u", i, (uint8_t)_msg.at(i));
        }
    }

    if(_vmsgs.size() > 5) {
        // Here we should wait until Arduino will have finished rebooting, 2 seconds will be enough
        QEventLoop _eloop;
        QTimer::singleShot(2000, &_eloop, SLOT(quit()));
        _eloop.exec();

        // Now we can send data
        qInfo("Передача временного интервала на контроллер...");
        _serialproc.writeToPort(_vmsgs[0]);
        // Make time delay that allows Arduino to parse setTime message, 0.01 seconds will be enough for 9600 bauds per second
        QTimer::singleShot(10, &_eloop, SLOT(quit()));
        _eloop.exec();

        qInfo("Передача количества каналов на контроллер...");
        _serialproc.writeToPort(_vmsgs[1]);
        // Make time delay that allows Arduino to parse setTime message, 0.01 seconds will be enough for 9600 bauds per second
        QTimer::singleShot(10, &_eloop, SLOT(quit()));
        _eloop.exec();

        qInfo("Передача уровня красного...");
        _serialproc.writeToPort(_vmsgs[2]);
        // Make time delay that allows Arduino to parse setTime message, 0.01 seconds will be enough for 9600 bauds per second
        QTimer::singleShot(10, &_eloop, SLOT(quit()));
        _eloop.exec();
        qInfo("Передача уровня зелёного...");
        _serialproc.writeToPort(_vmsgs[3]);
        // Make time delay that allows Arduino to parse setTime message, 0.01 seconds will be enough for 9600 bauds per second
        QTimer::singleShot(10, &_eloop, SLOT(quit()));
        _eloop.exec();
        qInfo("Передача уровня синего...");
        _serialproc.writeToPort(_vmsgs[4]);
        // Make time delay that allows Arduino to parse setTime message, 0.01 seconds will be enough for 9600 bauds per second
        QTimer::singleShot(10, &_eloop, SLOT(quit()));
        _eloop.exec();

        qInfo("Передача сигнала на контроллер...");
        _serialproc.writeToPort(_vmsgs[5]);

        // Make time delay before all 999 counts will be recieved by the Arduino, 5 seconds will be enough
        QTimer::singleShot(5000, &_eloop, SLOT(quit()));
        _eloop.exec();
    } else {
        qWarning("Файл с сигналом не соответсвует протоколу!");
    }
}

void MainWindow::aboutDialog()
{
    QDialog *aboutDialog = new QDialog(this);
    aboutDialog->setWindowTitle("О программе " + QString(APP_NAME));
    int _ps = this->font().pointSize();
    aboutDialog->setFixedSize(_ps*30, _ps*20);

    QVBoxLayout *tempLayout = new QVBoxLayout();

    QLabel *projectnameLabel = new QLabel(QString(APP_NAME) + "\t" + QString(APP_VERSION));
    projectnameLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    projectnameLabel->setAlignment(Qt::AlignCenter);
    QLabel *projectauthorsLabel = new QLabel(QString(APP_DESIGNER) /*+ "\n" + QString(APP_COMPANY) + "\n" + QString(APP_YEAR)*/);
    projectauthorsLabel->setAlignment(Qt::AlignCenter);

    QLabel *descriptionLabel = new QLabel(tr("Программа для загрузки сигналов\nв контроллер генератора\nсложноструктурированных сигналов"));
    descriptionLabel->setAlignment(Qt::AlignCenter);
    descriptionLabel->setWordWrap(true);

    QLabel *hyperlinkLabel = new QLabel(QString("<a href = '%1'>Страница проекта</a>").arg(QString(APP_WEB)));
    hyperlinkLabel->setToolTip(tr("Открыть в браузере"));
    hyperlinkLabel->setOpenExternalLinks(true);
    hyperlinkLabel->setAlignment(Qt::AlignCenter);

    tempLayout->addWidget(projectnameLabel);
    tempLayout->addWidget(projectauthorsLabel);
    tempLayout->addWidget(descriptionLabel);
    tempLayout->addWidget(hyperlinkLabel);

    aboutDialog->setLayout(tempLayout);
    aboutDialog->exec();

    delete hyperlinkLabel;
    delete projectauthorsLabel;
    delete projectnameLabel;
    delete tempLayout;
    delete aboutDialog;
}

void MainWindow::on_helpAction_triggered()
{
    aboutDialog();
}
