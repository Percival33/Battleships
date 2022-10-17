# Battleships game
Project for collage assignment essentials of Informatics and Programming
### Task
Write battleships game, then extend it by writing additional functionalities, so it will become interesting.

## Goals:

Main goals for the assignment

Basics:

- [x] Implement a board of fixed size 21x10 
- [x] Each player has exactly 1 ship of size 5 [CAR]RIER , 2 ships of size 4 [BAT]TLESHIP, 3 ships of size 3 [CRU]ISER , 4 ships of size 2 [DES]TROYER
- [x] There is a possibility of placing the ships by the players. A can put ships in rows 0...9; B can put ships in rows 11...20. 
- [x] The players can make moves, one after another, the only possible move is to either place the ships or shot one shot at a given position.
- [x] The game can detect which players wins. The checking of victory condition (the opposing player has no healthy ship fragments left) should be done at the end of the turn.
- [x] Program can print the state of the game.

Extended logic: 

- [x] The board can be of arbitrary size. The player starting positions can be bounded into arbitrary rectangle. There can be specified reefs, i.e. forbidden fields on the map and no ship under any circumstances can be present on such a field. The number of reefs specified can be quite large, but bounded by the size of board. Also, some of the ships of the players can be preallocated. Make the size and allocation of the board truly dynamic.
- [x] A player can make an order to move a ship. In each round each of the players can make any ship rotate, or move one field in its direction. Each ship except the carrier can move up to 3 times in a round. Carrier can move only up to 2 times. However, due to the fact that the ships are ships, they cannot rotate in place, and, for simplicity, are always going forward 1 field.  
E.g. for a destroyer changing course to the right twice.
```
#       #+++      #   +
#+# ->  # #  ->   # # +
#+      #         #   +
#+      #         #
```
- [x] Each of the ships contains additional section, radar at it's head. The radar provides an 
information about positions of other ships, i.e. if there is a opposing ship in the range of the 
radar. If the radar is damaged, then the range of the radar is 1; otherwise it is the length of the 
ship. A field is in range if the distance (the same formula as for the cannon) between the field 
and the radar is at most the range of the radar. Of course, a ship knows it's position always. 
Also, carriers can send spy planes (in number up to equal to the number of shoots - 5), the 
planes revel presence of enemy ships in 3x3 square each. Sending a planes count as shooting.
Printing from player's perspective should be implemented.

- [x]  Extended printing, in addition to the previous point the following things should be printed:
a) engine as %
b) cannon as !
c) radar as @
If the cannon and engine are taking the same field, then engine should be printed. Also, the 
numbers of rows and columns should be printed, e.g. as:
```
 00000000001
 01234567890
00
01
00 # @!++%
01
02 #%
03 @
```
- [x] Printing (Saving) the state of the game. The program should be able to print commands 
which allows to reconstruct the state of the game.

Advanced logic:

- [x] implement A.I. which should be able to execute some reasonable strategy. For example, to always move carrier out of the range of other ships, if possible; or to scout the map initially.

## Commands: 

**STATE COMMANDS**

The group of state commands starts with **[state]** and the commands from this group are provided until 
another **[state]** is encountered. There are

Basic: 
- ```PRINT x``` - **x = 0**  basic version,  **x = 1**   extended version
- ```SET_FLEET P a1 a2 a3 a4``` - The number of ships of consecutive classes for the player P
- ```NEXT_PLAYER P``` - Sets an information that P should make move next.

Extended:


- ```BOARD_SIZE y x``` - Set the board size to y x 
- ```INIT_POSITION P y1 x1 y2 x2``` The player P can place ships in rectangle given by (y1, x1) (left upper) and (y2, x2) (right lower).
- ```REEF y x``` - Place a reef at position (y,x)
- ```SHIP P y x D i C a1...al``` - There is a ship of player P at position (y,x) in the direction D(N/W/S/E) i-th of class C 
(CAR/BAT/CRU/DES). The state of the segments are a1 … al, where l is the length of the 
ship and 1 means that the part is not destroyed, 0 that it was destroyed. This command is 
unaffected by starting positions requirements.
- ```EXTENDED_SHIPS``` -Turn the extended logic of ships. I.e. the ships are composed of the 2 sections, etc. Turning 
this on disables the default SHOOT operations by players. 
- ```SAVE``` - Save the commands that allows to recreate the state of the game.


**PLAYER COMMANDS**

The group of player commands starts and ends with **[playerX]**, where X is either A or B. Commands 
are give until another corresponding **[playerX]** is given.

Basic:

- ```PLACE_SHIP y x D i C``` - Place the i-th ship of class C at (y,x) with direction equal to D.
- ```SHOOT y x``` - Shoot at the field (y,x). Shooting can only start if all the ships were placed.

Extended:

- ```MOVE i C x``` - Move the i-th ship of class C x ([F]orward, [L]eft, [R]ight).
- ```SHOOT i C y x``` -Let one of the shoots of i-th class ship be at (y,x).
- ```SPY i y x``` - Send a spy plane from i-th carrier at the position (y,x).
- ```PRINT x``` - Print the state of the game as seen by the player. That is, print the state as seen using radars 
and spy planes. The reefs are known every time. If a field is empty print ' '. If a field 
contains a visible part of ship print it. If a field is unknown print '?'.

