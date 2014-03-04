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

# pull barista config options from config file
stripe_size = 1024
chunk_size = 1024
barista_config = "config_file"

# configure cmd line args for barista node
barista_args = str(stripe_size) + " " + str(chunk_size) + " " + barista_config
barista_args = barista_args + " " + " ".join(espresso_ips)
print(barista_args)
# fork barista here
pids=[]

for (i, ip) in enumerate(espresso_ips):
   pid = os.fork()

   if pid == 0:
      espresso=subprocess.Popen(["ssh", "pi@" + ip, "./decafs_espresso"], stdout=sys.stdout)
      espresso.communicate()
      exit(0)
   else:
      pids.append(pid)

pid = os.fork()
if pid == 0: # if in child
   barista = subprocess.Popen(["ssh", "pi@" + barista_ip, "./decafs_barista" + " " + barista_args], stdout=sys.stdout)
   barista.communicate()
   exit(0)
else:
   pids.append(pid)

count = 0
while (count < len(pids)):
   os.waitpid(0, 0)
   count = count + 1
