#ifndef HANDLESTUFFSNESCLASSIC_H
#define HANDLESTUFFSNESCLASSIC_H

#include "handlestuff.h"

#include <snesclassicstuff/desktopclient/miniftp.h>
#include <snesclassicstuff/desktopclient/telnetconnection.h>

class HandleStuffSnesClassic : public HandleStuff
{
public:
    HandleStuffSnesClassic();
    void            setCommandCo(TelnetConnection* co, TelnetConnection *canoe);

protected:
    QByteArray      saveState(bool trigger);
    void            loadState(QByteArray data);
    bool            hasScreenshots();
    bool            hasShortcutsEdit();

private:
    TelnetConnection*        telCo;
    TelnetConnection*        canoeCo;
    MiniFtp*                 ftpCo;
    QByteArray               lastLoadMD5;

    void        runCanoe(QStringList canoeArgs);
    QStringList getCanoeExecution();
    void        killCanoe(int signal);
    void removeCanoeUnnecessaryArg(QStringList &canoeRun);
};

#endif // HANDLESTUFFSNESCLASSIC_H