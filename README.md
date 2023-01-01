# gate-monitor

A Particle project named gate-monitor

This just needs an LCD connected to I2C port and listens to the cloud for main_gate_1 and clubhouse_gate_1 events and prints an open/closed state when it sees those.

(https://github.com/djb-rh/gate-monitor/blob/master/gate-monitor.jpg)

## Details

This project is intended to control gates connected to a Particle Photon plugged into an NCD.io relay board and running the [gate-controller](https://github.com/djb-rh/gate-controller) firmware written by Donnie Barnes (and based on the NCD IOT firmware for those boards).

It has two buttons and uses the clickButton library for debouncing and long versus short press events. Each button is illuminated via a neopixel LED to show gate status within the physical button. So when the gate is open, the button for that gate is green. When the gate is closed, that button will show as red. There is a "main" gate as well as a "club" gate.

There is also a 4 line by 20 character LCD that shows the gate status. This was originally intended as the only output method, but the neopixels were added later. Then we added another function called "skips", explained below. But basically only two lines are used, one for each gate showing the status (open/closed) and how many skips are current for each gate.

We currently use IFTTT to trigger gates on a schedule, but sometimes we want to skip the next IFTTT command (or more than one). This is what skips do.

How? Will first let's discuss how it all works. Everything is listening to the greater particle cloud. To open the main gate and have it stay open, all one needs to do is publish "main_gate_relay_1 1on" to set the first relay on the main gate controller to on, which will open that gate. To close it, "main_gate_relay_1 1off". When a state is changed, the gate doing the change, for example main_gate, will publish main_gate_1 "ON" or "OFF" depending on the state. And if you want to ask the cloud what the gate status is, any device can publish "getstate" and each gate will respond with the status.

If you want to tell a gate to skip a command, there is only a way to tell EVERY gate to skip the next X commands. You publish "skipcommand X" where X is a SINGLE DIGIT number (we only support setting 0-9 skips). So if you want main_gate to skip the next command and you want club_gate to NOT skip a command, you simply need to publish "skipcommand 1" and then publish any command to the club_gate (so a "club_gate_relay_1 1off" would do it, and the "1off is simply ignored).

I have no recollection of why the gate names end in underscore-one. It just is what it is and I'm too lazy to fix it.

## Compiling your project

When you're ready to compile your project, make sure you have the correct Particle device target selected and run `particle compile <platform>` in the CLI or click the Compile button in the Desktop IDE. The following files in your project folder will be sent to the compile service:

- Everything in the `/src` folder, including your `.ino` application file
- The `project.properties` file for your project
- Any libraries stored under `lib/<libraryname>/src`
