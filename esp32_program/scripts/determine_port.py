import subprocess
import sys
import os

def print_colored(text, color, end='\n'):
    colors = {'red': '\x1b[31m', 'green': '\x1b[32m', 'yellow': '\x1b[33m', 'blue': '\x1b[34m'}
    reset = '\x1b[0m'
    sys.stdout.write(colors.get(color, '') + text + reset + end)

def scan_ports_mac_addresses(env, desired_mac):
    # Path to the shell script
    script_path = "scripts/check_mac_address.sh"
    print_colored("desired_mac\t" + desired_mac, "green")

    # Check if the script can be accessed and is executable
    if os.access(script_path, os.X_OK) != True:
        print_colored(f"Cannot open the script '{script_path}'. It may not exist or is not executable.\n", "red")
        env.Exit(1)

    # Run the shell script to check MAC addresses
    result = subprocess.run(script_path + " " + desired_mac, capture_output=True, text=True, shell=True)

    # Check if the script executed successfully
    if result.returncode == 0:
        print_colored("Script executed successfully.\n", "green")
        print_colored("Output:\t" + result.stdout, "green")
    else:
        print_colored("Script failed to execute.\n", "red")
        print("Error Code:", result.returncode)
        print_colored(script_path + " output:\n" + result.stdout, "green")
        env.Exit(1)

    upload_port = result.stdout.strip()
    monitor_port = upload_port

    print_colored("Uploading to port:\t" + upload_port, color='green')

    env.Replace(UPLOAD_PORT=upload_port)
    env.Replace(MONITOR_PORT=monitor_port)
