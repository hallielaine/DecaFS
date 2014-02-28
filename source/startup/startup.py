#!/usr/bin/python

import argparse
import json
#from ssh import SSHClient
import subprocess 
import sys

parser = argparse.ArgumentParser(description='startup script for DecaFS')
parser.add_argument('-f',
                    default=False, 
                    action='store_true', 
                    help='set this flag if a new instance of DecaFS')

parser.add_argument('--config',
                    '-c',
                    type=str,
                    required='True',
                    help='set this flag if a new instance of DecaFS')

args = parser.parse_args()
#print(args.f)
#print(args.config)

json_data = open(args.config).read()
data = json.loads(json_data)

espresso_ips=[]
barista_ip=None

if data["nodes"]["barista"]["ip"]:
  barista_ip=data["nodes"]["barista"]["ip"]

for node in data["nodes"]["espresso"]:
  espresso_ips.append(node["ip"])

# deal with handling incorrecly formatted config files here

#subprocess.check_output(["ssh", "pi@" + barista_ip, "touch testfile"])
barista = subprocess.Popen(["ssh", "pi@" + barista_ip, "echo Barista is: `cat /etc/hostname`"], stdout=sys.stdout)
barista.communicate()

espresso=[]
for (i, ip) in enumerate(espresso_ips):
   espresso.append(subprocess.Popen(["ssh", "pi@" + ip, "echo Espresso " + str(i+1) + ": `cat /etc/hostname`"], stdout=sys.stdout))
   espresso[len(espresso)-1].communicate()
