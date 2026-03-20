# POP Raylib starter

Simple starter application for POP C assignment.

Please see materials on Blackboard for more details.

# Building

To build run the command:

```bash
/opt/pop/bin/build-wasm.sh src/main.c
```

This will generate a directory *out* with the WASM and index.html files for the 
Raylib program.

# Running

The very first time you run a POP WASM application you must run the command:

```bash
/opt/pop/bin/allocate_port.sh
```

You might need to start a new terminal instance for the update to take effect.
To check that everything is fine run the command:

```bash
echo $MY_PORT
```

This should output a 5 digit number.


To run the Raylib program in *out* simply run the command:

```bash
/opt/pop/bin/run-wasm.sh
```

This will run a web server that serves the *out* on the port you allocated above. This is forwarded from the 
remote server to your local machine, which means you can simply open the corresponding web page within a browser 
on your local machine using the address:

```bash
localhost:XXXXX
```

where *XXXXX* is the port number you allocated above.


# Everbloom

Everbloom is a small magical gardening and alchemy game built in C with Raylib.

The player takes on the role of a grower asked to cultivate the legendary Everbloom, a rare flower tied to strange and dangerous outcomes. Players select two seeds, plant them, wait for the flowers to grow through different stages, harvest them, and finally brew a potion in the cauldron. Different seed combinations lead to different potion colours and endings.

## Features

- Title screen and story dialogue
- Inventory system for selecting two seeds
- Planting and flower growth system
- Harvesting mechanic
- Cauldron brewing system
- Multiple potion outcomes with dialogue

## Controls

- **Mouse click** to interact with buttons, seeds, plot, and dialogue
- **I** to open or close inventory
- **ESC** to close inventory
- **C** to clear selected seeds
- **B** to move to brewing after harvesting
- **R** to restart after the ending, if implemented
