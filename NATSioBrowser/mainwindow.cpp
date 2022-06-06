#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "bizlogic.h"

#include <QDateTime> //for dbug output

#include <QSettings>    //save-load ini
#include <QFileDialog>
    /*
     * CONNESC SIGNAL => SLOT
     */
void MainWindow::initExtSignalsIntSlots(){
        //output client::dbug
    connect(&iBizLogicNats, SIGNAL(outputDbug(QString)), this, SLOT(printDbug(QString)) );
        //output mainWindow::dbug
    connect(this, SIGNAL(outputDbug(QString)), this, SLOT(printDbug(QString)) );
        //show status MQ
    connect(&iBizLogicNats, SIGNAL(signalOpenCompliteMQ()), this, SLOT(connectIs()) );
    connect(&iBizLogicNats, SIGNAL(signalDisconnectMQ()), this, SLOT(disconnectIs()) );

}

void MainWindow::printDbug(QString aSrc){
    QString out="\r\n";
        QDate date = QDate::currentDate(); QString dateString = date.toString("dd.MM");
    out+=(" [" + dateString);
        QTime time = QTime::currentTime(); QString timeString = time.toString("hh:mm:ss.zzz");
    out+=(";" + timeString+"]");
    out+=aSrc;
    out+="\r\n";
    ui->weLogsOut->insertPlainText(out);
    ui->weOutputSubscr->insertPlainText(out);
    ui->statusbar->showMessage(out);
}

void MainWindow::dbug(QString aS){
    emit outputDbug(aS);
}

void MainWindow::connectIs(){
   ui->wbConnect->setChecked(true);
   ui->wrStatusMQ->setChecked(true);
}

void MainWindow::disconnectIs(){
   ui->wbConnect->setChecked(false);
   ui->wrStatusMQ->setChecked(false);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    initExtSignalsIntSlots();
    //
    ui->setupUi(this);
    //after load gui
    openDefaultIni();

}


MainWindow::~MainWindow()
{
    if (ui->wchbAutosaveExit->isChecked()==true){
        MainWindow::on_wpbSaveini_pressed();
    }
    delete ui;
}
        /*
         * AUTO-LOAD-SAVE INI
         */
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

                        ui->weHostNameNATSio->setText(settings.value("HostMqttFw").toString());
                        ui->wvPortNATSio->setValue(settings.value("PortMqttFw").toUInt());
                        ui->weClientId->setText(settings.value("ClientIdMqttFw").toString());
                        ui->weUName->setText(settings.value("UserMqttFw").toString());
                        ui->weUPasswd->setText(settings.value("PasswdMqttFw").toString());
                        ui->checkBoxNoSaveAccount->setChecked(settings.value("checkBoxNoSaveAccount").toBool());
                        ui->weTopicPub->setText(settings.value("TopicPub").toString());
                        ui->wePayload->setText(settings.value("PayoladPub").toString());
                        ui->weTopicSub->setText(settings.value("TopicSubscr").toString());
                        ui->wchbAutosaveExit->setChecked(settings.value("autosaveExit").toBool());
                        //show
                        ui->tabWidget->setCurrentIndex(settings.value("TabIndexShow").toUInt());

                        settings.endGroup();

                }


                void MainWindow::on_wpbSaveini_pressed()
                {
                    on_PathIniFileChanged();

                    QSettings settings(ui->PathIniFile->text(),QSettings::IniFormat);

                        settings.beginGroup("Settings");

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
                    settings.setValue( "TopicPub", ui->weTopicPub->text());
                    settings.setValue( "PayoladPub", ui->wePayload->text());
                    settings.setValue( "TopicSubscr", ui->weTopicSub->text());
                    settings.setValue( "checkBoxNoSaveAccount", ui->checkBoxNoSaveAccount->checkState());
                    settings.setValue( "autosaveExit", ui->wchbAutosaveExit->checkState());
                    //show
                    settings.setValue( "TabIndexShow", ui->tabWidget->currentIndex());

                        settings.endGroup();
                }
/*
 *  TAB-EXPLORE TAB-EXPLORE TAB-EXPLORE TAB-EXPLORE TAB-EXPLORE TAB-EXPLORE TAB-EXPLORE TAB-EXPLORE TAB-EXPLORE TAB-EXPLORE
 */
/*
 *  SET SUBSCREABLE ALL TEXT TOPIC '>'
 */
void MainWindow::on_wbAllTopics_pressed()
{
    ui->weTopicSub->setText(">");
}

void MainWindow::on_wbConnect_pressed() //CONNECT BUTTON
{
    QString host=ui->weHostNameNATSio->text();
    quint16 port=ui->wvPortNATSio->value();

    iBizLogicNats.openNATSio(host,port);
}

void MainWindow::on_wbClose_pressed()   //CLOSE BUTTON
{
    iBizLogicNats.closeNATSio();
}

void MainWindow::on_wbSubscr_pressed()
{
    QString strTopicSub=ui->weTopicSub->text();
    uint64_t ssid = iBizLogicNats.subscrNATSio(strTopicSub);
    dbug( "subscrible topic: '" + strTopicSub + "', let ssid:" + QString::number(ssid) );
}


void MainWindow::on_wbUnScrible_pressed()
{
#warning todo ssid
//    client->unsubscribe(ssid,max_massege);
}

/*
 * public payload@topic to natsio queue
*/
void MainWindow::on_wbPublic_pressed()
{
//    if (iBizLogicNats.iClient.isConnect()asdfasdfasdfas==NULL)
//        on_wbConnect_pressed();
    QString
            strTopicPub     = ui->weTopicPub->text();
    QString
            payload         = ui->wePayload->text();
    //
    iBizLogicNats.publishNATSio(strTopicPub, payload);
//    dbug("..");
//         ".. Publish payload:'"+payload
//         +"' at topic: '"+strTopicPub+"'"
//         );
}


void MainWindow::on_sizeFornReser_pressed()
{
    ui->weOutputSubscr->setFontWeight(10);
    ui->weOutputSubscr->setFontWeight(10);
}


void MainWindow::on_sizeFontUp_pressed()
{
    ui->weOutputSubscr->setFontWeight(ui->weOutputSubscr->fontWeight()*1.1);
}


void MainWindow::on_sizeFontDn_pressed()
{
    ui->weOutputSubscr->setFontWeight(ui->weOutputSubscr->fontWeight()/1.1);
}

