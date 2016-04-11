## Raspberry Pi version

### Dependencies:
* WiringPi

### Running
1. Link and compile to an executable
2. Create `/feeder` directory in `/home` and move the executable there
3. Edit the system's `rc.local` and add the executable as `/home/feeder/executable &`
4. (Optional) Create a `fido.conf` in `/feeder` and store the mode, dispense frequency (in minutes), and empty state; each on a new line

The program will now run autonomously on boot, as long as appropriate power is supplied to the Pi and the motor
