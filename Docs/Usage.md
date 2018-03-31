# Link-TLM Usage #

Using `Link-TLM` is pretty simple. Setting up the radio properly is the most critical part to getting 
`Link-TLM` up and running right, and so I'll also cover that here!

## Setting Up - Radio ##

There are a few components to the radio, so to make sure all the terms are straight, refer to this list:

- **Radio**: The box that usually sits on the floor of the tracking van, which the antenna, face, and power cord are connected to.
- **Antenna**: The tall thin antenna which sits on top of the van; _not_ the patch or other antennas used for communicating with payloads
- **Face**: The handheld display that connects to the radio. Sometimes called the **panel**.
- **TNC Cable**: The cable which connects the face to the computer running `Link-TLM`. It has a DIN (round) connector on one end, and a USB on the other, with a connector in the middle (DB-9, old-school serial).

The radio must be a TM-D710GA to work with `Link-TLM`.

### Hardware Setup ###

First get the antenna on top of the tracking van and run its cable in through a window. Try to go through
the closest window to where the tracker will be sitting. Screw the antenna into the radio. Connect the 
face to the radio with its phone-like cable (ethernet-like cable). Plug the radio's power cable into an
available cigarette lighter port. 

Finally, and _most critically for `Link-TLM`_, plug the TNC cable _into the back of the face_. Not the
radio, the face. **Not the _radio_, the _face_.** _This is the most common mistake I have seen in setup_.
If you plug the cable into the radio, _`Link-TLM` will do (and can do) nothing._

### Radio Settings ###

Power on the radio using the power button on the face. Verify that the frequency of one of the radio's 
channels is set to 144.390. Channels can be switched by "clicking" the two smaller knobs - click the left
one to swtich to the channel on the left of the screen and vice versa. Change frequency by turning the 
larger knob on the left. At this point, shut down the radio (using the same power button) and leave the 
rest until you get to the launch site.

### Verify Callsigns ###

Once the trackers in the command module are powered on, power on the radio and wait for a packet from each
callsign in the command module. When the radio recieves a packet, it displays the callsign on the face.

### Hook Up to Tracking Laptop ###

First, set the "packet12" mode on the radio: Press the button next to the "TNC" label on the screen until
the label `PACKET12` appears in the top left of the screen. This allows the radio to send packets to the
computer.

Next, determine what port the radio is on when plugged in to the laptop:
- Before plugging the TNC cable in, open a terminal on the laptop and run `ls /dev/tty*`
- Then plug in the TNC cable and run the same command.
- A new entry will have showed up. That one is the radio.

## Running Link-TLM ##

- In the terminal, `cd` into the `Link-TLM` directory. If the `Link-TLM` directory is on the desktop, for 
example, run `cd ~/Desktop/Link-TLM`.
- Run `./Link-TLM` with the name of the radio port you got above, i.e. `./Link-TLM /dev/ttyUSB0`.
- _Ignore warnings about van callsigns_. At the moment, `Link-TLM` does nothing with these, but provision 
for them was added. This warning means nothing.

After the above, `Link-TLM` currently sits and waits for packets. If nothing is happening, _the program has
not frozen_. It just isn't receiving anything. Listen for the radio's characteristic blip when receiving
to tell when `Link-TLM` should see output; _only if_ you hear that blip and `Link-TLM` does nothing should
you worry.

When it does receive a packet, `Link-TLM`:

- Logs the raw data.
- Decides if it is from a callsign that we want to capture
    - If not, it will display an `Untracked Callsign` message.
    - If it is, it will clear the terminal and display all relevant data gathered from the packet.

To close `Link-TLM`, simply type "q" and hit enter in the terminal.

_Where to next? You should check out [Live Data From Link-TLM](./LiveData.md)_
