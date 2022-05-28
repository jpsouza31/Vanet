//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
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

#include "veins/modules/application/traci/TraCIDemoRSU11p.h"

#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

using namespace veins;

Define_Module(veins::TraCIDemoRSU11p);

void TraCIDemoRSU11p::onWSA(DemoServiceAdvertisment* wsa)
{
    // if this RSU receives a WSA for service 42, it will tune to the chan
    if (wsa->getPsid() == 42) {
        mac->changeServiceChannel(static_cast<Channel>(wsa->getTargetChannel()));
    }
}

void TraCIDemoRSU11p::onWSM(BaseFrame1609_4* frame)
{
    TraCIDemo11pMessage* wsm = check_and_cast<TraCIDemo11pMessage*>(frame);

    if ( myId == wsm->getSerial() ) {
            std::cout << "###### Sou o node " << myId << " e recebi a mensagem ######" << endl;
    }
    //std::cout << "## Recebi a mensagem: " << wsm->getDemoData() << " do node: " << wsm->getSenderAddress() << " no tempo: " << simTime() << endl;
    //for (int i = 0; i < 100000000; i++) {
    //    if (i%10000000 == 0) {
    //        std::cout << "## Processando tarefa" << endl;
    //    }
    //}
    //std::cout << "## Enviando tarefa processada para " << wsm->getSenderAddress() << endl;
    //TraCIDemo11pMessage* message = new TraCIDemo11pMessage();
    //populateWSM(message, 28);
    //message->setSenderAddress(myId);
    //std::string dado = "Tarefa processada";
    //message->setDemoData(dado.c_str());
    //sendDown(message);
}
