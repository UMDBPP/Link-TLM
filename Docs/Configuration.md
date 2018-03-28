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

The to row of the Configurator has three buttons.

- `Load Settings` will pull in the current `Prefs/settings.json` file.
    - This will fail and throw a Python error in the terminal if there is no settings file.
    - However, the Configurator will not crash - so you can build one from scratch!
- `Reset Defaults` will force back to a default state, which is not very good, but is at least valid.
    - Launch number is set to 0, for example.
- `Save Settings` will dump your current settings to the `Prefs/settings.json` file
    - This option is a bit smart - it will dump the last _valid_ settings out. So if you have an error - it won't be saved!
    - However, the compromise is that your "last valid settings" probably aren't what you want.
    - You can always do a quick `Load Settings` to pull in the new file and confirm its contents!
