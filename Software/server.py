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

# Function to handle key press event
def on_press(key):
    if key.name in key_actions:
        action = key_actions[key.name]
        send_command(action)

# Function to handle key release event
def on_release(key):
    if key.name in key_actions:
        send_command('N')  # Send 'N' command when the key is released

# Register the key press and release events
for key in key_actions.keys():
    keyboard.on_press_key(key, on_press)
    keyboard.on_release_key(key, on_release)

# Run the keyboard listener loop
keyboard.wait('esc')  # Press 'esc' to exit the program