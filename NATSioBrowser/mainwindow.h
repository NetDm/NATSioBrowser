#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "bizlogic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void outputDbug(QString);

private slots:
    void on_wpbOpenDialogini_pressed();

    void on_pbReloadini_pressed();

    void on_wpbSaveini_pressed();

    void on_wbAllTopics_pressed();

    void on_wbSubscr_pressed();

    void on_wbConnect_pressed();

    void on_wbClose_pressed();

    void on_wbUnScrible_pressed();

    void on_wbPublic_pressed();

    void on_sizeFornReser_pressed();

    void on_sizeFontUp_pressed();

    void on_sizeFontDn_pressed();

public slots:
    void printDbug(QString aSrc);
        //show state open MQ
    void connectIs();
    void disconnectIs();

private:
    Ui::MainWindow *ui;
    void dbug(QString);

    BizLogic iBizLogicNats;

    void initExtSignalsIntSlots();


    void openDefaultIni();
    void on_PathIniFileChanged();

};
#endif // MAINWINDOW_H
