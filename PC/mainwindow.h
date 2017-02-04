#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTextBrowser>

#include "qspprocessor.h"

//------------------------------------------------------------------------------------------
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
//------------------------------------------------------------------------------------------

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_selectfileAction_triggered();
    void on_searchportsAction_triggered();

    void on_pushButton_clicked();

private:
    void __adjustactionstotoolbuttons();

    Ui::MainWindow *ui;

    QSPProcessor spproc;
};

#endif // MAINWINDOW_H
