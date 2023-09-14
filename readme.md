
WARNING: THIS IS VERY ROUGH WORK IN PROGRESS

TODO:

- fix this documentation
- add presets for known cartridges
- add more error checking / user input validation
- perhaps add some sort of automatic scanning for known patterns

-------------------------------------------------------------------------------

![gtkunidec](/images/gtkunidec.png)

unidec is a simple command line tool to decode ROM dumps from ROMs that have
their address- and/or data lines scrambled on the hardware.

gtkunidec is an interactive version of the same program, which hopefully is
helpful with working out how the lines are connected (when the hardware is not
available for doing measurements).

gtkunidec is somewhat c64 centric, the two text columns show "screencode" and
"PETSCII" strings. it should be very easy to modify for whatever other type of
encoding however.

-------------------------------------------------------------------------------

building:

just type "make". gtkunidec will require GTK3 libraries installed.

-------------------------------------------------------------------------------

C64 Expansion port (looking "into" the Computer)

    22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1
    == == == == == == == == == == == == == == == == == == == == == ==
    == == == == == == == == == == == == == == == == == == == == == ==
     Z  Y  X  W  V  U  T  S  R  P  N  M  L  K  J  H  F  E  D  C  B  A

     1  GND     Ground
     2  +5V DC  VCC
     3  +5V DC  VCC
     4  !IRQ    In      Interrupt
     5  R/_W    Out     Read or _Write indicator
     6  DOT Clk Out     Dot clock frequency (7.881984 MHz for PAL, 8.181816 MHz for NTSC)
     7  !IO1    Out     Signal is low if address bus is within $DE00-$DEFF.
     8  !GAME   In      Game configuration
     9  !EXROM  In      External ROM configuration
    10  !IO2    Out     Signal is low if address bus is within $DF00-$DFFF.
    11  !ROML   Out     Select signal for ROM in range $8000-$9FFF
    12  BA      Out     BA=0 means VIC needs the bus during both phases of PHI2 and no other unit can use it.
    13  !DMA    In      if !DMA=Low the CPU can be requested to release the bus.
    14  D7      In/Out  Data line 7
    15  D6      In/Out  Data line 6
    16  D5      In/Out  Data line 5
    17  D4      In/Out  Data line 4
    18  D3      In/Out  Data line 3
    19  D2      In/Out  Data line 2
    20  D1      In/Out  Data line 1
    21  D0      In/Out  Data line 0
    22  GND     Ground

     A  GND     Ground
     B  !ROMH   Out     Select signal for ROM in range $E000-$FFFF
     C  !RESET  In      If this line is pulled to low, all chips will be re-initialized.
     D  !NMI    In      Non Maskable Interrupt
     E  PHI2    Out     Phase 2 System clock (0.98524861 MHz for PAL, 1.02272714 MHz for NTSC)
     F  A15     In/Out  Address line 15
     H  A14     In/Out  Address line 14
     J  A13     In/Out  Address line 13
     K  A12     In/Out  Address line 12
     L  A11     In/Out  Address line 11
     M  A10     In/Out  Address line 10
     N  A9      In/Out  Address line 9
     P  A8      In/Out  Address line 8
     R  A7      In/Out  Address line 7
     S  A6      In/Out  Address line 6
     T  A5      In/Out  Address line 5
     U  A4      In/Out  Address line 4
     V  A3      In/Out  Address line 3
     W  A2      In/Out  Address line 2
     X  A1      In/Out  Address line 1
     Y  A0      In/Out  Address line 0
     Z  GND     Ground

28 pin DIL IC:

      28 27 26 25 24 23 22 21 20 19 18 17 16 15
    --O--O--O--O--O--O--O--O--O--O--O--O--O--O--
    |                                          |
    |                                          |
    ))                                         |
    |                                          |
    |                                          |
    --O--O--O--O--O--O--O--O--O--O--O--O--O--O--
      1  2  3  4  5  6  7  8  9  10 11 12 13 14

      2764 (8Kib) EPROM:          23128 (16Kib) mask ROM:            27256 (32Kib) EPROM:

      1 Vpp     28  Vcc           1 (nc)    28  Vcc                  1 Vpp     28  Vcc
      2 A12     27  PGM           2 A12     27  OE1/!OE1/nc          2 A12     27  A14
      3 A7      26  (nc)          3 A7      26  A13                  3 A7      26  A13
      4 A6      25  A8            4 A6      25  A8                   4 A6      25  A8
      5 A5      24  A9            5 A5      24  A9                   5 A5      24  A9
      6 A4      23  A11           6 A4      23  A11                  6 A4      23  A11
      7 A3      22  OE/!G         7 A3      22  OE/!OE/nc            7 A3      22  !G
      8 A2      21  A10           8 A2      21  A10                  8 A2      21  A10
      9 A1      20  CS/!E         9 A1      20  CE/!CE/OE2/!OE2      9 A1      20  !E
     10 A0      19  D7           10 A0      19  D7                  10 A0      19  D7
     11 D0      18  D6           11 D0      18  D6                  11 D0      18  D6
     12 D1      17  D5           12 D1      17  D5                  12 D1      17  D5
     13 D2      16  D4           13 D2      16  D4                  13 D2      16  D4
     14 GND     15  D3           14 GND     15  D3                  14 GND     15  D3

-------------------------------------------------------------------------------

The following is a collection of info on how the ROMs on various C64 cartridges
are connected - if you can provide more, please let me know!

StarDOS:
--------

Datalines D1 and D2 are swapped

The ROM is 16k, the cartridge uses a 27128 EPROM

StarDOS (1541 ROM):
-------------------

Addresslines A10 and A11 are swapped

The ROM is 16k, the drive board uses a 27128 EPROM

Capture:
--------

The capture ROM is 8k, there is a 2764 EPROM on the cartridge

    Expansionport     EPROM 2764
    Y   A0                A0    10
    X   A1                A1    9
    W   A2                A2    8
    V   A3                A10   21
    U   A4                A3    7
    T   A5                A4    6
    S   A6                A11   23
    R   A7                A5    5
    P   A8                A9    24
    N   A9                A6    4
    M   A10               A8    25
    L   A11               A7    3
    K   A12               A12   2

    21  D0                D3    15
    20  D1                D2    13
    19  D2                D4    16
    18  D3                D1    12
    17  D4                D5    17
    16  D5                D0    11
    15  D6                D6    18
    14  D7                D7    19


s'more/kingsoft business basic:
-------------------------------

???

printing modul:
---------------

Contains a 27128 EPROM (16k)

    Cartridge port     Chip pin
    Y/A0                A0
    X/A1                A1
    W/A2                A2
    V/A3                A3
    U/A4                A4
    T/A5                A5
    S/A6                A6
    R/A7                A7
    P/A8                A12
    N/A9                A10
    M/A10               A11
    L/A11               A9
    K/A12               A8
    J/A13               A13

    21/D0               D2
    20/D1               D1
    19/D2               D3
    18/D3               D0
    17/D4               D5
    16/D5               D7
    15/D6               D4
    14/D7               D6


Silverrock 128
--------------

    Cartridge port     Chip pin
    Y/A0               A10
    X/A1               A11
    W/A2               A9
    V/A3               A8
    U/A4               A0
    T/A5               A1
    S/A6               A2
    R/A7               A3
    P/A8               A4
    N/A9               A5
    M/A10              A6
    L/A11              A7
    K/A12              A12

    21/D0              D3
    20/D1              D2
    19/D2              D1
    18/D3              D0
    17/D4              D4
    16/D5              D5
    15/D6              D6
    14/D7              D7

for the bank switching, a register in the IO1 area is used. the 4 topmost data
bits are used in this order:

      D4               A14
      D5               A15
      D6               A16
      D7               A13

on the v2 hardware, apparently the lower 4 bits of the address that is written
to is used instead:

      A0               A13
      A1               A14
      A2               A15
      A3               A16


Warpspeed 128
-------------

Contains a 28 pin mask ROM (16k)

    Cartridge port     Chip pin
    Y/A0                A0
    X/A1                A1
    W/A2                A10
    V/A3                A2
    U/A4                A3
    T/A5                A11
    S/A6                A4
    R/A7                A9
    P/A8                A5
    N/A9                A8
    M/A10               A6
    L/A11               A7
    K/A12               A13
    J/A13               A12

    21/D0               D3
    20/D1               D2
    19/D2               D4
    18/D3               D1
    17/D4               D0
    16/D5               D5
    15/D6               D6
    14/D7               D7

Whizzard
--------

Contains a 28 pin mask ROM (16k)

    Cartridge port     Chip pin
    Y/A0               13 ? no idea
    X/A1               9 ? no idea
    W/A2               8 ? no idea
    V/A3               11 ? no idea
    U/A4                A10 guessed
    T/A5                A7 guessed
    S/A6                A6
    R/A7                A5
    P/A8                A4
    N/A9                A0
    M/A10               A1
    L/A11               A2
    K/A12               A3
    J/A13               A12

    21/D0               ? no idea
    20/D1               ? no idea
    19/D2               ? no idea
    18/D3               ? no idea
    17/D4               ? no idea
    16/D5               ? no idea
    15/D6               ? no idea
    14/D7               ? no idea
