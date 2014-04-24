#!/usr/bin/python

import argparse
import json
import subprocess 
import sys
import os
import time

import signal

from subprocess import PIPE, Popen

fp = open('output', 'w')

def preexec_function(): pass

# setup cmd line args
parser = argparse.ArgumentParser(description='startup script for DecaFS')
parser.add_argument('--first',
                    '-f',
                    default=False, 
                    action='store_true', 
                    help='set this flag if a new instance of DecaFS')

parser.add_argument('--config',
                    '-c',
                    type=str,
                    required='True',
                    help='set this flag if a new instance of DecaFS')

args = parser.parse_args()

# load json configuration file
json_data = open(args.config).read()
data = json.loads(json_data)
#print(data)

espresso_nodes=[]
espresso_ips=[]
barista_node=None
barista_ip=None
system_port=None

if data["system_port"]:
  system_port = data["system_port"]

if data["barista"]:
  barista_node=data["barista"]
  barista_ip=barista_node["ip"]

for node in data["espresso"]:
  espresso_nodes.append(node)
  espresso_ips.append(node["ip"])

# deal with handling incorrecly formatted config files here

stripe_size = data["stripe_size"]
chunk_size = data["chunk_size"]

# configure cmd line args for barista node
barista_args = str(stripe_size) + " " + str(chunk_size) + " " + barista_node["metadata"]
barista_args = barista_args + " " + str(len(espresso_nodes))
print(barista_args)

b_proc=None
e_procs=[]

ip = barista_node["ip"]
metadata = barista_node["metadata"]
print("executing decafs_barista on " + ip)
b_proc = subprocess.Popen(["ssh", "-t", "-t", ip, "./decafs_barista", barista_args], stdout=sys.stdout, stderr=sys.stderr)

time.sleep(1)

for (i, node) in enumerate(espresso_nodes):
   #TODO: check to make sure these exist
   ip = node["ip"]
   node_id = node["id"]
   metadata = node["metadata"]
   filesystem = node["filesystem"]

   print("executing decafs_espresso on " + ip)
   e_procs.append(subprocess.Popen(["ssh", "-t", "-t", ip, "./decafs_espresso", node_id, metadata, filesystem, barista_ip, system_port], stdin=open(os.devnull), stdout=sys.stdout, stderr=sys.stderr))

b_proc.communicate()
print("decafs_barista exited")

for proc in e_procs:
   proc.send_signal(signal.SIGINT) 
   proc.wait()
