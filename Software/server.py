import requests
import keyboard

# Define the base URL for controlling the robot
base_url = "http://192.168.4.1/"

# Define the dictionary to map keys to corresponding actions
key_actions = {
    'w': 'F',  # Forward
    's': 'B',  # Backward
    'a': 'L',  # Left
    'd': 'R',  # Right
    'space': 'N'  # Stop
}

# Function to send HTTP requests to control the robot
def send_command(action):
    url = base_url + action
    try:
        response = requests.get(url)
        if response.status_code == 200:
            print(f"Command sent successfully: {action}")
        else:
            print(f"Failed to send command: {action}. Status code: {response.status_code}")
    except Exception as e:
        print(f"An error occurred: {e}")

# Listen for keyboard input to control the robot
for key, action in key_actions.items():
    keyboard.add_hotkey(key, send_command, args=(action,))

# Run the keyboard listener loop
keyboard.wait('esc')  # Press 'esc' to exit the program