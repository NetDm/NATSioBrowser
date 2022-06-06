#ifndef BIZLOGIC_H
#define BIZLOGIC_H

#include "natsio/natsclient.h"
#include <QObject>

//#define dbug(aSrc) emit dbug(aSrc);


using namespace Nats;//clients

class BizLogic : public QObject
{
    Q_OBJECT
public:
    explicit BizLogic(QObject *parent = nullptr);

    Nats::Client iClient;        //instance/экземпляр, класс соединения
    Nats::Subscription iSubscr;//instance/экземпляр, класс подписки

    void openNATSio(QString aHost, quint16 aPort);
    void closeNATSio();

    uint64_t subscrNATSio(QString aTopic);

    void publishNATSio(QString aTopic,QString aPayload,int QoS);
    void publishNATSio(QString aTopic,QString aPayload);

signals:
    void outputDbug(const QString);
    void signalOpenCompliteMQ();
    void signalDisconnectMQ();

private:
    void dbug(QString);

    void initSignalsSlots();
public slots:
    //conn-disc NATSio
    void slotCompliteOpenNatsHost(Nats::Client *);
    void slotDisconnectNatsHost(Nats::Client *);
    void slotErrorNatsio(const QString error);
    //pub-sub
//    void slotComplitePublicNatsHost(Nats::Subscription *);
    void slotReciveSubscrNatsHost(Nats::Subscription *);

};

#endif // BIZLOGIC_H
