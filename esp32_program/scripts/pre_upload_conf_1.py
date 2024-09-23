
from determine_port import scan_ports_mac_addresses
import json
import subprocess
import sys
import os

Import("env")

# # Load the configuration
# with open('config.json') as config_file:
#     config = json.load(config_file)
#     desired_mac = config['DESIRED_MAC_1']

desired_mac = env.GetProjectOption("desired_mac")

print(f"scanning port for: {desired_mac}")

scan_ports_mac_addresses(env, desired_mac)