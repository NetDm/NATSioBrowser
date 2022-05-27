#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "natsio/natsclient.h"

#include <QSettings>
#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->tabWidget);

    openDefaultIni();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openDefaultIni(){
    QSettings settPath ("settPath.ini",QSettings::IniFormat);
    settPath.beginGroup("Path");

    ui->PathIniFile->setText( settPath.value("FilePathDefault").toString() );
    on_pbReloadini_pressed();
    settPath.endGroup();
}

void MainWindow::on_PathIniFileChanged()
{
    QSettings settPath ("settPath.ini",QSettings::IniFormat);
    settPath.beginGroup("Path");

    settPath.setValue( "FilePathDefault", ui->PathIniFile->text());

    settPath.endGroup();
}


void MainWindow::on_wpbOpenDialogini_pressed()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open INI File"),
                                "", tr("INI Files (*.ini *.conf)"));
     if (!filePath.isEmpty()) {

         ui->PathIniFile->setText(filePath);
         on_PathIniFileChanged();
         on_pbReloadini_pressed();
     }

}


void MainWindow::on_pbReloadini_pressed()
{
    QSettings settings (ui->PathIniFile->text(),QSettings::IniFormat);

        settings.beginGroup("Settings");

//        ui->editFileNameFw->setText(settings.value("fwFilePath").toString());
        ui->weHostNameNATSio->setText(settings.value("HostMqttFw").toString());
        ui->wvPortNATSio->setValue(settings.value("PortMqttFw").toUInt());
        ui->weClientId->setText(settings.value("ClientIdMqttFw").toString());
        ui->weUName->setText(settings.value("UserMqttFw").toString());
        ui->weUPasswd->setText(settings.value("PasswdMqttFw").toString());
//        ui->TopicsFw->setText(settings.value("TopicsFw").toString());
//        ui->BlockSizeFw->setValue(settings.value("BlockSizeFw").toString().toUInt());
//        ui->TopicSizeFw->setText(settings.value("TopicSizeFw").toString());
//        ui->TopicCrc32Fw->setText(settings.value("TopicCrc32Fw").toString());
//        ui->editRevPcb->setText(settings.value("topicRevPcb").toString());
//        ui->editRevPcb_2->setText(settings.value("editRevPcb").toString());
//        ui->checkBoxAutoSave->setChecked(settings.value("autoSaveCheck").toBool());
        ui->checkBoxNoSaveAccount->setChecked(settings.value("checkBoxNoSaveAccount").toBool());

        settings.endGroup();

}


void MainWindow::on_wpbSaveini_pressed()
{
    on_PathIniFileChanged();

    QSettings settings(ui->PathIniFile->text(),QSettings::IniFormat);

        settings.beginGroup("Settings");

//        settings.setValue( "fwFilePath", ui->editFileNameFw->text());
        settings.setValue( "HostMqttFw", ui->weHostNameNATSio->text());
        settings.setValue( "PortMqttFw", ui->wvPortNATSio->text());
        settings.setValue( "ClientIdMqttFw", ui->weClientId->text());
    if (ui->checkBoxNoSaveAccount->isChecked()==false){
            settings.setValue( "UserMqttFw", ui->weUName->text());
            settings.setValue( "PasswdMqttFw", ui->weUPasswd->text());
    }else{
            settings.setValue( "UserMqttFw", "");
            settings.setValue( "PasswdMqttFw", "");
    }
//        settings.setValue( "TopicsFw", ui->TopicsFw->text());
//        settings.setValue( "BlockSizeFw", ui->BlockSizeFw->text());
//        settings.setValue( "TopicSizeFw", ui->TopicSizeFw->text());
//        settings.setValue( "TopicCrc32Fw", ui->TopicCrc32Fw->text());
//        settings.setValue( "topicRevPcb", ui->editRevPcb->text());
//        settings.setValue( "editRevPcb", ui->editRevPcb_2->text());
//        settings.setValue( "autoSaveCheck", ui->checkBoxAutoSave->checkState());
//        settings.setValue( "checkBoxNoSaveAccount", ui->checkBoxNoSaveAccount->checkState());

        settings.endGroup();
}

