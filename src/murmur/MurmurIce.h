#include "murmur_pch.h"

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#define ICE_QEVENT (QEvent::User + 959)

class MurmurIce : public QObject {
  friend class MurmurLocker;
  Q_OBJECT;
  protected:
    int count;
    QMutex qmEvent;
    QWaitCondition qwcEvent;
    Ice::CommunicatorPtr communicator;
    void customEvent(QEvent *evt);
  public:
    MurmurIce();
    ~MurmurIce();
};

