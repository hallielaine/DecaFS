#!/usr/bin/python

import argparse
import json
import subprocess 
import sys
import os

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

# fork barista here
pids=[]

pid = os.fork()
if pid == 0: # if in child
   #barista = subprocess.Popen(["ssh", "pi@" + barista_ip, "echo Barista is: `cat /etc/hostname`"], stdout=sys.stdout)
   barista = subprocess.Popen(["ssh", "pi@" + barista_ip, "./decafs_barista"], stdout=sys.stdout)
   barista.communicate()
   exit(0)
else:
   pids.append(pid)

for (i, ip) in enumerate(espresso_ips):
   #espresso.append(subprocess.Popen(["ssh", "pi@" + ip, "echo Espresso " + str(i+1) + ": `cat /etc/hostname`"], stdout=sys.stdout))
   pid = os.fork()

   if pid == 0:
      espresso=subprocess.Popen(["ssh", "pi@" + ip, "./decafs_espresso " + ip], stdout=sys.stdout)
      espresso.communicate()
      exit(0)
   else:
      pids.append(pid)

count = 0
while (count < len(pids)):
   os.waitpid(0, 0)
   count = count + 1
