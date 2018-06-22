#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include <qcolorgenerator.h>

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
    void on_durationD_valueChanged(int value);

    void on_actionExit_triggered();

    void on_selectsignalB_clicked();

    void on_selectfileB_clicked();

    void on_durationD_sliderReleased();

    void on_runsessionB_clicked();

    void on_stackedWidget_currentChanged(int arg1);

private slots:
    void endSession();

private:
    Ui::MainWindow *ui;
    void setupFooterTB();
    void setupHeadTB();
    void setupFileSystemTreeView();
    void loadSettings();
    void saveSettings();    

    QSettings *settings;
    QString signalfilename;
    QColorGenerator qcolorgen;
};

#endif // MAINWINDOW_H
