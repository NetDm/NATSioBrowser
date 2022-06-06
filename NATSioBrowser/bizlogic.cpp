#include "bizlogic.h"
#include <QCoreApplication>

/*
 * LOGIC NATSio CLIENT
 */

#include "natsio/natsclient.h"

BizLogic::BizLogic(QObject *parent) : QObject(parent){
    initSignalsSlots();
}

void BizLogic::initSignalsSlots(){
    //
    Nats::Client* pClient;
    pClient = &iClient;
    void(*fOutputDbug)(Nats::Client*);

    iClient.QObject::connect(&iClient , &Nats::Client::connected , [=]{slotCompliteOpenNatsHost(&iClient);});

    iClient.QObject::connect(&iClient, &Nats::Client::error
                             ,[=](const QString &error){/*slotErrorNatsio(error);*/} );

    iClient.QObject::connect(&iClient , &Nats::Client::disconnected , [=]{ slotDisconnectNatsHost(&iClient); });

}

void BizLogic::dbug(QString aS){
    emit outputDbug(aS);
}
/*
 * CONNECTING-DISCONNCT MQ NATS  CONNECTING-DISCONNCT MQ NATS  CONNECTING-DISCONNCT MQ NATS  CONNECTING-DISCONNCT MQ NATS
*/
void BizLogic::slotErrorNatsio(QString aError){
    dbug("!!err: NATS client:"+aError);
}

void BizLogic::slotCompliteOpenNatsHost(Nats::Client* aClient){
    dbug("+ Open NATSio host complite!");
    if (aClient!= &iClient){
        dbug("Current ptr NatsClient does not match the instance at callback complite BizLogic::slotCompliteOpenNatsHost");
    }
    emit signalOpenCompliteMQ();
}

void BizLogic::openNATSio(QString aHost, quint16 aPort){
    dbug(".. Trying to open MQ NATS host service: '"+aHost+"':"+QString::number(aPort)+" wait ...");
    iClient.connect( aHost , aPort );
}


void BizLogic::slotDisconnectNatsHost(Nats::Client* aClient){
    dbug("- Disconnect NATSio host complite!");
    if (aClient!= &iClient){
        dbug("Current ptr NatsClient does not match the instance at callback BizLogic::slotDisconnectNatsHost");
    }
    emit signalDisconnectMQ();
}

void BizLogic::closeNATSio(){
    dbug(".. Trying disconnect MQ NATS, wait ...");
    iClient.disconnect();
}
/*
 * SUBSCRIBLE MQ  SUBSCRIBLE MQ  SUBSCRIBLE MQ  SUBSCRIBLE MQ  SUBSCRIBLE MQ  SUBSCRIBLE MQ
*/
uint64_t BizLogic::subscrNATSio(QString aTopic){

    Nats::Subscription* subscr = iClient.subscribe(aTopic);

            Nats::Subscription::QObject::connect(subscr  ,  &Nats::Subscription::received  ,  [=]{ slotReciveSubscrNatsHost(subscr); }  );

    dbug("+ Subscrible MQ NATS topic:'"
         +aTopic
         +"'--> ssid:"
         +QString::number( subscr->ssid )
         +""
            );
    return ( subscr->ssid );
}

void BizLogic::slotReciveSubscrNatsHost(Nats::Subscription* aSubscr){
    QString mess="> Recive MQ Subscrible { Payload:'"
                + QString(aSubscr->message)
                + "', Topic:'"
                + QString(aSubscr->inbox)
                + "', ssid:["
                +QString::number(aSubscr->ssid)
                + "], subject:'"
                + QString(aSubscr->subject)
                +"' }"
            ;
    dbug(mess);
//    emit signalOpenCompliteMQ();
}

//void BizLogic::slotComplitePublicNatsHost(Nats::Subscription* aClient){
//    QString mess="! Complite Public MQ Subscrible ";
//    dbug(mess);
////    emit signalOpenCompliteMQ();
//}

void BizLogic::publishNATSio(QString aTopic,QString aPayload){
    dbug(
         "< Publish payload:'"+aPayload
         +"' at topic: '"+aTopic+"'"
         );
    iClient.publish(aTopic,aPayload);
}
