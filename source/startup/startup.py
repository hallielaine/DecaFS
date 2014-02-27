#!/usr/bin/python

import argparse
import json
#from ssh import SSHClient
import subprocess 

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
print(args.f)
print(args.config)

json_data = open(args.config).read()
data = json.loads(json_data)

espresso_ips=[]
barista_ip=None

if data["nodes"]["barista"]["ip"]:
  barista_ip=data["nodes"]["barista"]["ip"]
  print('barista: ' + barista_ip)

for node in data["nodes"]["espresso"]:
  espresso_ips.append(node["ip"])

for (i, ip) in enumerate(espresso_ips):
   print('espresso ' + str(i) + ': ' + ip)

# deal with handling incorrecly formatted config files here

#barista=SSHClient()
#barista.load_system_host_keys()
#barista.connect(barista_ip) # may need more args
#stdin, stdout, stderr = barista.exec_command('ls')

subprocess.check_output(["ssh", "pi@" + barista_ip, "touch testfile"])

