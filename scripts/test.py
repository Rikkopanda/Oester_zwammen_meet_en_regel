import sys
import os

def print_colored(text, color, end='\n'):
    colors = {'red': '\x1b[31m', 'green': '\x1b[32m', 'yellow': '\x1b[33m', 'blue': '\x1b[34m'}
    reset = '\x1b[0m'
    sys.stdout.write(colors.get(color, '') + text + reset + end)

print_colored('red text', color='red')

# Example using colorama to print colored text in Python
import colorama
from colorama import Fore, Style
colorama.init(autoreset=True)  # Initializes colorama and autoresets color

# Function to print messages in different colors
def print_colored_message(message, color):
    if color == 'red':
        print(Fore.RED + message)
    elif color == 'green':
        print(Fore.GREEN + message)
    elif color == 'blue':
        print(Fore.BLUE + message)
    else:
        print(message)  # Default to no color


# Example usage
print_colored_message('This is a warning message.', 'red')
print_colored_message('This is an informational message.', 'green')
print_colored_message('This is just a regular message.', 'none')

# This function demonstrates how to use colorama to print text in different colors.
# It's a simple way to enhance the readability of terminal output by differentiating
# information through colors. The function takes a message and a color as arguments,
# and prints the message in the specified color.