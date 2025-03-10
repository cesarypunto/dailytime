# DailyTime for Pebble

## Description
**DailyTime** is a C application for Pebble watches that helps manage time spent on daily activities (dailies). This app provides a configurable timer to assist users in better managing their time.

## Features
- Customizable timer with predefined times.
- Vibration notifications upon timer completion.
- Simple and optimized interface for Pebble's screen.
- Pause, reset, and real-time adjustment options.

## Requirements
- A compatible Pebble watch.
- Pebble SDK installed in the development environment.
- Basic knowledge of C and Pebble development.

## Installation
1. Clone this repository:
   ```sh
   git clone https://github.com/your_user/daily-timer-pebble.git
   ```
2. Navigate to the project directory:
   ```sh
   cd dailytime
   ```
3. Compile the application using the Pebble SDK:
   ```sh
   pebble build
   ```
4. Install the application on your Pebble watch:
   ```sh
   pebble install --phone <PHONE_IP>
   ```

## Usage
1. Launch the app from the Pebble applications menu.
2. Press the start button to begin the countdown.
3. Receive a vibration notification when the time is up.
4. Pause, reset as needed.

## License
This project is licensed under the MIT License. See the `LICENSE` file for more details.
