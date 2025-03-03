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

### Configuring build

This project has been written assuming that you don't have an Arduino in hand to program it. So, that's why it defaults on creating fake Arduino libraries so that the compiler doesn't complain about missing header files or undeclared functions. This, however, allows you to paste the entire code into an IDE without having to include the missing Arduino related functions. Pretty neat!

If you want to disable this feature when building because you are programming in Arduino IDE or similar, then just add the `TESTING=false` flag at the end of the `make` command, like so:

```bash
make TESTING=false
```

This will prevent the fake libraries from loading and it will use instead the real ones. It will also modify some functions and introduce randomness to simulate real data.

### Modes

There are different execution modes: *mapping* and *manual*

#### Mapping

This mode is useful when testing, to enable it, make sure that the `TEST` flag is set to `true`, it should be the default tho.

This mode is automatic in the sense that there is no user interaction. It will scan the environment, which it will populate with random obstacles. This mode is meant to be expanded with a pahtfinding algorithm such as [Dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm), which doesn't depend on knowing the goal location, unlike [A*](https://en.wikipedia.org/wiki/A*_search_algorithm). Another very efficent option could be [Depth Fist Search (DFS)](https://en.wikipedia.org/wiki/Depth-first_search), which when used to pathfind, likes to slalom, which is what the course shape is intended to be, making it probably the fastest algorithm that you could use.

#### Manual

The manual mode is meant to be tested on the ground, with a real arduino connected to a computer, or alternatively in a simulating software like [Tinkercad](https://tinkercad.com). To use it make sure to build the program with:

```bash
make TESTING=false
```

This mode allows you to enter the movement of the rover by entering it as input in the Serial Monitor. This requires the Arduino to be plugged to a computer though. The mode comes with many interesting features such as:

- Self-spinning
- Independent sensor movement
- Input controlled movement
- Customizable detection range
- LED lights to display movement status
- Safety mechanism that avoids near obstacles

To control it, just type the commands that appear in the Serial Monitor when triggering the Arduino, submited by pressing `Enter`. They are as follows:

```
f: forward
b: backward
r: right
l: left
```

Although they can be remapped to [WASD](https://en.wikipedia.org/w/index.php?title=Arrow_keys#WASD_keys) to provide a AAA game experience.


## Credits

To the endless threads and posts on: Reddit, Stack Overflow, W3Schools and the Arduino Forum. Most of the features wouldn't be here without their amazing guides and explanations. :)