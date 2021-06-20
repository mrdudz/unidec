

TODO:

- fix this documentation
- add presets for known cartridges
- add more error checking / user input validation



StarDOS:
--------

Datalines D1 and D2 are swapped

StarDOS (1541 ROM):
-------------------

Addresslines A10 and A11 are swapped


Capture:
--------

Expansionport     EPROM 2764
Y   A0 ---------      A0    10
X   A1 ---------      A1    9
W   A2 ---------      A2    8
V   A3 ---------      A10   21
U   A4 ---------      A3    7
T   A5 ---------      A4    6
S   A6 ---------      A11   23
R   A7 ---------      A5    5
P   A8 ---------      A9    24
N   A9 ---------      A6    4
M   A10 --------      A8    25
L   A11 --------      A7    3
K   A12 --------      A12   2
J   A13
H   A14
F   A15

21  D0 ---------      D3    15
20  D1 ---------      D2    13
19  D2 ---------      D4    16
18  D3 ---------      D1    12
17  D4 ---------      D5    17
16  D5 ---------      D0    11
15  D6 ---------      D6    18
14  D7 ---------      D7    19


s'more/kingsoft business basic:
-------------------------------

???

printing modul:
---------------


    @datamap = {
      # Bus-Bit => Chip-Bit
      7 => 6,
      6 => 4,
      5 => 7,
      4 => 5,
      3 => 0,
      2 => 3,
      1 => 1,
      0 => 2,
    }
    @addrmap = {
      # Bus-Bit => Chip-Bit
      0 => 0,
      1 => 1,
      2 => 2,
      3 => 3,
      4 => 4,
      5 => 5,
      6 => 6,
      7 => 7,
      
      8 => 12,
      9 => 10,
      10 => 11,
      11 => 9,
      12 => 8,
      13 => 13,
    }




Warpspeed 128
-------------

Cartridge port     Chip pin --- 
Y/A0                A0 --------- 
X/A1                A1 --------- 
W/A2                A10 -------- 
V/A3                A2 --------- 
U/A4                A3 --------- 
T/A5                A11 -------- 
S/A6                A4 --------- 
R/A7                A9 --------- 
P/A8                A5 --------- 
N/A9                A8 --------- 
M/A10               A6 --------- 
L/A11               A7 --------- 
K/A12               A13 -------- 
J/A13               A12 -------- 


21/D0               D3 --------- 
20/D1               D2 --------- 
19/D2               D4 --------- 
18/D3               D1 --------- 
17/D4               D0 --------- 
16/D5               D5 --------- 
15/D6               D6 --------- 
14/D7               D7 --------- 

