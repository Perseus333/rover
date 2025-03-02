# M.E.M.E.
*(Mechanized Experimental Mars Explorer)*

Obstacle avoidance software for an arduino powered rover.

## Building

### 1. Cloning the repository
If you want to build the source yourself, you can do so by running this following command:

```bash
git clone --depth 1 --recurse-submodules --shallow-submodules https://github.com/Perseus333/MEME.git
```

What it does is it clones only the latest commit from this github repository (hence `--depth 1`) and it includes all the submodules. I'm assuming that you are not using this inside of the Arduino IDE, so that's why I added the Arduino Core and Servo libraries as submodules. You can find here: [AduinoAvr-core](https://github.com/arduino/ArduinoCore-avr) and [Servo](https://github.com/arduino-libraries/Servo)  This means that depening if you are on Arduino IDE or not, you can choose to skip download the exernal libraries, or download them. You can toggle the sumodules by including the `--recurse-submodules`. Since you will probably only want the latest release and not the whole commit history, `--shallow-submodules` enables that. It's like the `--depth 1` but for submodules.

### 2. Building the source

Assuming that you just have executed the last command, you will need to navigate to the folder for the code that you have written by running:

```bash
cd MEME
```

Now that you are in the root folder of the project, you can finally build it with the `make` command.

```bash
make
```

## Usage

After you have built the source, you can execute the program by executing the binary which will be stored in the `/bin` directory. You can run it with:

`./bin/main.exe`

### Using flags

If you would like to change some of the variables at compile time, to test with different configurations you can do so effortlessly by specifying them then building the code. The variables that are modifiable should appear in the `makefile`. If you don't specify them, they will default to their fallback values.

You can specify them when you run the code by writing:

```bash
make SCANS_PER_SWIPE=8 MAX_MEMORY=100
```
