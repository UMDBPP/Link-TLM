# Link-TLM Configuration #

`Link-TLM` now comes with a lovely GUI configurator! Here's how to use it.

## Install ##

`Link-TLM-Configurator` is a Python 3 script which uses `wxPython` for its GUI. Unfortunately,
`wxPython` requires a bit of love to get working on Debian...sorry about that :/

Please see the [INSTALL](./Install.md) file for instructions.

## Usage ##

First of all: This application will irrevocably edit the settings file found in `Prefs/settings.json`!
If you _only_ use this app, you should be fine, and always end up with a valid settings file. **HOWEVER!**
**If you edit `Prefs/settings.json` manually, I cannot guarantee proper operation!!!**

Now then, onto the app!

### Running ###

It's as simple as `cd Link-TLM && ./Link-TLM-Configurator`. The GUI should pop up!
(If you're already in the `Link-TLM` directory, you can just `./Link-TLM-Configurator`.
The app _does_ count on operating from the directory, however, so you must be in it.)

### The Top Row ###

The top row of the Configurator has three buttons.

- `Load Settings` will pull in the current `Prefs/settings.json` file.
    - This will fail and throw a Python error in the terminal if there is no settings file.
    - However, the Configurator will not crash - so you can build one from scratch!
- `Reset Defaults` will force everything back to a default state, which is not very good, but is at least valid.
    - Launch number is set to 0, for example.
    - The reset is not automatically saved - you must click save
- `Save Settings` will dump your current settings to the `Prefs/settings.json` file
    - This option is a bit smart - it will dump the last _valid_ settings out. So if you have an error - it won't be saved!
    - However, the compromise is that your "last valid settings" probably aren't what you want.
    - You can always do a quick `Load Settings` to pull in the new file and confirm its contents!

### The Fillable Fields ###

The fields below the top row are where you actually edit the settings for `Link-TLM`. Note that these
are not a one-to-one mapping with the settings file. This makes the settings more intuitive to edit
and also enforces a unified filename convention.

The values in the fields are validity checked in real-time; if a value is not okay, its field will
turn red to let you know something's wrong. `Link-TLM-Configurator` will not save settings that are invalid.

- `Launch Number` is the ID number of the launch, with no dash. Example: `NS50`
    - This field checks for the format of "NS" followed by numbers.
    - Lowercase "ns" and other letters will not work.
    - The rules on this field exist to unify log names & ensure logs for each launch  are accounted for.
- `Callsigns` is a comma-separated list of the callsigns _for the main string_.
    - Callsigns are validated using the exact same regex as `Link-TLM` uses internally.
    - Therefore, validity of this rule is key to ensure `Link-TLM` works properly.
    - Dropped payloads, multiple balloons, and vans are _not supported_ right now.
        - If you put any of these in, `Link-TLM` will confuse them with the main string, and your data will be nonsensical.
        - These _cannot_ be validated against - callsigns are an operational decision; it is the tracker's job to ensure the right ones are used.
    - Examples of valid entries: `W3EAX-9` or `W3EAX-12,W3EAX-10` or `KB3ZZI-10,W3EAX-10`
        - Props if anyone remembers who KB3ZZI belongs to - only a few upperclassmen have met him at this point.
- `Raw Packet Log Folder` is a relative or absolute path to a directory where the raw packet log file is, for example `Logs/` or `/home/nick/Logs/`
    - This log contains everything the radio receives, from all callsigns, but in raw APRS packet format.
    - The full filename will be `Path/to/log/<Launch Number>_allPackets.txt`
    - Default, and what I always used, was the `Logs/` directory in the `Link-TLM` folder.
    - `~` for home directories is not supported - may fix this at a later date. Python limitation.
    - To pass validity checks, the path:
        - Must end in a `/`
        - Must already exist on the computer
        - Must be writeable (User running must have write access)
    - The above checks ensure the log will be written without error, so they are important to pass.
- `Parsed Packet Log Folder` is a relative or absolute path to a directory where the parsed packet log file is.
    - This log contains a csv formatted log of the parsed and derived data from `Link-TLM`.
    - The full filename will be `Path/to/log/<Launch Number>_parsedPackets.txt`
    - Default, and again what I always used, was the `Logs/` directory in the `Link-TLM` folder.
    - Validity checks, restrictions, and reasoning are the same as for the Raw field above.
- `JSON Log Folder` is a relative or absolute path to a directory where the json log will be saved.
    - This log contains the same data as the parsedPackets csv, but in JSON format.
    - The full filename will be `Path/to/log/<Launch Number>_packets.json`
    - Default is `Logs/` as above, but the impetus for this format was to place it on a local-area webserver via the wifi hotspot.
        - So, if you have write access to the webserver on the tracking laptop (`/var/www/` most likely), this is worth considering.
    - Validity checks, restrictions, and reasoning are still the same as Raw field above.
- `KML Folder` is a relative or absolute path to the directory where the KML (Google Earth) file will be saved.
    - This file contains a live-updating KML which can be loaded into Google Earth for a visualization of the flight.
        - See [Live Data](./LiveData.md) for details.
    - The full filename will be `Path/to/log/<Launch Number>_groundTrack.kml`
    - Default is `Logs/` as above, but as with the JSON, the idea is to share this with the van over wifi. Consider putting on local server.
    - Validity checks, restrictions, and reasoning are still the same as for the Raw field above.

That's really it! Once all the boxes are filled out and not red, **make sure you hit save**, and then `Link-TLM` is ready to go!

_Where to next? You probably want [Running Link-TLM](./Usage.md)_
