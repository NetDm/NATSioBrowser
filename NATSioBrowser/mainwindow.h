#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//user
#include "natsio/natsclient.h"
//
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_wpbOpenDialogini_pressed();

    void on_pbReloadini_pressed();

    void on_wpbSaveini_pressed();

private:
    Ui::MainWindow *ui;
    //user
    Nats::Client *client;

    void openDefaultIni();
    void on_PathIniFileChanged();    
};
#endif // MAINWINDOW_H
