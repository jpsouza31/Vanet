//
// Copyright (C) 2006-2011 Christoph Sommer <christoph.sommer@uibk.ac.at>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "veins/modules/application/traci/TraCIDemo11p.h"

#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

using namespace veins;

Define_Module(veins::TraCIDemo11p);

TraCIDemo11pMessage* TraCIDemo11p::createMessage (int to, int senderId, std::string dado) {
    TraCIDemo11pMessage* wsm = new TraCIDemo11pMessage();
    populateWSM(wsm, -1);
    wsm->setSerial(to);
    wsm->setSenderAddress(senderId);
    wsm->setDemoData(dado.c_str());
    wsm->setTll(simTime() + 0.01);
    return wsm;
}

void TraCIDemo11p::createAndSendMessage (int to, int senderId, simtime_t beginTime = NULL) {
    std::string dado = "Mensagem para o node " + std::to_string(to);
    TraCIDemo11pMessage* wsm = createMessage(to, myId, dado);
    if (beginTime == NULL) {
        wsm->setBeginTime(simTime());
    }
    sendDown(wsm);
//    std::cout << "## Enviado a mensagem para o " << to << endl;
}

void TraCIDemo11p::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
        messageSendInterval = 20;
        timeToSendMessage = simTime() + messageSendInterval;
        rsuIds[0] = 16;
        rsuIds[1] = 21;
        rsuIds[2] = 41;
        rsuIds[3] = 27;
        rsuIds[4] = 31;
        rsuIds[5] = 36;
        rsuIds[6] = 46;
    }
}

void TraCIDemo11p::onWSA(DemoServiceAdvertisment* wsa)
{
    if (currentSubscribedServiceId == -1) {
        mac->changeServiceChannel(static_cast<Channel>(wsa->getTargetChannel()));
        currentSubscribedServiceId = wsa->getPsid();
        if (currentOfferedServiceId != wsa->getPsid()) {
            stopService();
            startService(static_cast<Channel>(wsa->getTargetChannel()), wsa->getPsid(), "Mirrored Traffic Service");
        }
    }
}

void TraCIDemo11p::onWSM(BaseFrame1609_4* frame)
{
    TraCIDemo11pMessage* wsm = check_and_cast<TraCIDemo11pMessage*>(frame);
//    std::cout << "## Sou o node " << myId << " e a mensagem é para o - " << wsm->getSerial() << endl;
    if ( myId == wsm->getSerial() ) {
        wsm->setEndTime(simTime());
        simtime_t delay = wsm->getEndTime() - wsm->getBeginTime();
//        string data = "ID: " + myId + " - delay: " + delay;
        std::ofstream out("arquivo.txt");
        out << myId + ' - ' + delay;
        out.close();
        std::cout << "## AQUI";
//        std::cout << "## Sou o node " << myId << " e recebi a mensagem com tempo de atraso - " << delay << endl;
    } else if ( simTime() > wsm->getTll() ){
//        std::cout << "######## Acabou o tempo " << endl;
        return;
    } else {
        createAndSendMessage(wsm->getSerial(), myId, wsm->getBeginTime());
//        std::cout << "## Tentando reencaminhar para o node " << wsm->getSerial() << endl;
    }
}

void TraCIDemo11p::handleSelfMsg(cMessage* msg)
{
    if (TraCIDemo11pMessage* wsm = dynamic_cast<TraCIDemo11pMessage*>(msg)) {
        // send this message on the service channel until the counter is 3 or higher.
        // this code only runs when channel switching is enabled
        sendDown(wsm->dup());
        wsm->setSerial(wsm->getSerial() + 1);
        if (wsm->getSerial() >= 3) {
            // stop service advertisements
            stopService();
            delete (wsm);
        }
        else {
            scheduleAt(simTime() + 1, wsm);
        }
    }
    else {
        DemoBaseApplLayer::handleSelfMsg(msg);
    }
}

void TraCIDemo11p::handlePositionUpdate(cObject* obj)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);

    if (myId % 4 == 0 && simTime() == timeToSendMessage) {
        int rsuId = rsuIds[rand() % 7];
        createAndSendMessage(rsuId, myId, NULL);
        timeToSendMessage = timeToSendMessage + messageSendInterval;
    }
}
