#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on Mar 26, 2013

@author: Guilherme
'''

import os
from bs4 import BeautifulSoup

# Relative to sumo-launchd.py directory
LAUNCHD_BASE_DIR = "../SUMO-Manhattan-1km_x_1km/"
RANDOM_TRIP_FILE = "python /Users/Guilherme/Documents/OMNet/workspace/sumo-0.17.0/tools/trip/randomTrips.py"
NETWORK_FILE_NAME = "network.net.xml"
TRAFFICS = (20, 40, 60, 80, 100, 150, 200, 250, 300, 350, 400, 450, 500)
REPLICATIONS = 50

def build_route_files():
    for traffic in TRAFFICS:
        for replication in range(REPLICATIONS):
            # create routes files
            random_trip_cmd = RANDOM_TRIP_FILE + " -n " + NETWORK_FILE_NAME + " -b 0 -e " + str(traffic) + " -s " + str(replication) + " -r " + str(traffic) + "_" + str(replication) + ".rou.xml"
            print random_trip_cmd
            os.system(random_trip_cmd)                
            
            # modify start time for route files
            f = open(str(traffic) + "_" + str(replication) + ".rou.xml", "r")
            data = f.read()
            f.close()
            
            parser = BeautifulSoup(data, "xml")
            
            for e in parser.findAll("vehicle"):
                e["depart"] = "0.00"        
            f = open(str(traffic) + "_" + str(replication) + ".rou.xml", "w")
            f.write(parser.prettify())
            f.close()
            
            # remove alternative route files
            os.system("rm " + str(traffic) + "_" + str(replication) + ".rou.alt.xml trips.trips.xml")
            
def build_cfg_files():
    for traffic in TRAFFICS:
        for replication in range(REPLICATIONS):
            soup = BeautifulSoup(
                    """<?xml version="1.0"?>
                        <configuration>
                            <input>
                                <net-file value="" />
                                <route-files value="" />
                                <additional-files value="obstacles.poly.xml" />
                                <time-to-teleport value="-1" />
                            </input>
                            <time>
                                <begin value="0" />
                                <end value="1000" />
                            </time>
                        </configuration>""", "xml")
            
            net_tag = soup.find("net-file")
            net_tag["value"] = NETWORK_FILE_NAME
            
            route_tag = soup.find("route-files")
            route_tag["value"] = str(traffic) + "_" + str(replication) + ".rou.xml"                                                   
            
            f = open(str(traffic) + "_" + str(replication) + ".sumo.cfg", "w")
            f.write(soup.prettify())
            f.close()
            
def build_launchd_files():
    for traffic in TRAFFICS:
        for replication in range(REPLICATIONS):
            soup = BeautifulSoup(
                    """<?xml version="1.0"?>
                        <launch>
                            <basedir path="" />
                            <copy file="" />
                            <copy file="" />
                            <copy file="" type="config" />
                            <copy file="obstacles.poly.xml" />
                        </launch>""", "xml")
            
            basedir_tag = soup.find("basedir")
            basedir_tag["path"] = LAUNCHD_BASE_DIR
            
            copy_tags = soup.find_all("copy")
            copy_tags[0]["file"] = NETWORK_FILE_NAME
            copy_tags[1]["file"] = str(traffic) + "_" + str(replication) + ".rou.xml"
            copy_tags[2]["file"] = str(traffic) + "_" + str(replication) + ".sumo.cfg"
                                                                                                    
            f = open(str(traffic) + "_" + str(replication) + ".launchd.xml", "w")
            f.write(soup.prettify())
            f.close()                                 
            
if __name__ == "__main__":
    #build_route_files()
    #build_cfg_files()
    #build_launchd_files()
