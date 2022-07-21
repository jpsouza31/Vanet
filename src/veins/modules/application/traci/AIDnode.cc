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

#include "veins/modules/application/traci/AIDnode.h"

#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

using namespace veins;

Define_Module(veins::AIDnode);

void AIDnode::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
    }
}

void AIDnode::onWSA(DemoServiceAdvertisment* wsa)
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

void AIDnode::onWSM(BaseFrame1609_4* frame)
{
    TraCIDemo11pMessage* wsm = check_and_cast<TraCIDemo11pMessage*>(frame);
    std::cout << "## Recebi a mensagemMMMMMMS: " << wsm->getDemoData() << " do node: " << wsm->getSenderAddress() << " no tempo: " << simTime() << endl;
    std::cout << "## A mensagem é para o node: " << wsm->getSerial() << endl;
    if ( myId == wsm->getSerial() ) {
        std::cout << "## Sou o node " << myId << " e recebi a mensagem" << endl;
    } else if ( simTime() > 155.001 ){
        std::cout << "######## Acabou o tempo " << endl;
        return;
    } else {
        TraCIDemo11pMessage* wsmNew = new TraCIDemo11pMessage();
        populateWSM(wsmNew, -1);
        wsmNew->setSerial(12);
        wsmNew->setSenderAddress(myId);
        std::string dado = "Mensagem para o node 12";
        wsmNew->setDemoData(dado.c_str());
        sendDown(wsmNew);
        std::cout << "## Tentando reencaminhar para o node " << wsmNew->getSerial() << endl;
    }
}

void AIDnode::handleSelfMsg(cMessage* msg)
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

void AIDnode::handlePositionUpdate(cObject* obj)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);
    if (simTime() == 155) {
        if (myId == 28) {
            TraCIDemo11pMessage* wsm = new TraCIDemo11pMessage();
            populateWSM(wsm, -1);
            wsm->setSerial(12);
            wsm->setSenderAddress(myId);
            std::string dado = "Mensagem para o node 12";
            wsm->setDemoData(dado.c_str());
            sendDown(wsm);
            std::cout << "## Enviado a mensagem para o 12" << endl;
        }
    }
}
