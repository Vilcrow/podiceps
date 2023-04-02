Copyright (C) 2022-2023 Vilnur Shakirov

1. About
podiceps - a simple program for maintaining a dictionary of foreign words.

2. License
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.

3. Compiling
3.1. Linux
    For compiling under Linux or other Unix variants, the following is required:
        - GCC;
        - STL (Standard Template Library);
        - Qt 5.15.3 or later;
        - cmake 3.16 or later;

    Building:
        1) apt install build-essential qtbase5-dev qttools5-dev cmake
        2) mkdir build/ && cd build/
        3) cmake ../
        4) cmake --build .

4. Keyboard shortcuts
4.1 Main Window
    Ctrl + N - create a new file
    Ctrl + O - open file
    Ctrl + S - save file
    Ctrl + Shift + S - save file as
    Ctrl + I - show information about the current file
    Ctrl + W - quit the program
    Ctrl + Alt + Z - undo the table action
    Ctrl + Alt + Y - redo the table action
    Ctrl + Shift + A - open the add widget
    Ctrl + F - open the find widget
    Alt + F - close the find widget
    Ctrl + Shift + F - close the find widget
    Ctrl + Shift + E - open the words edit window
    Delete - delete selected rows
    Ctrl + P - open the preferences
    Ctrl + Shift + N - show the statistics
    Esc - deselect the table rows and clear input
4.2 Input Widget
    Enter - add a word
    Ctrl + Z - undo
    Ctrl + Shift + Z - redo
    Ctrl + X - cut
    Ctrl + C - copy
    Ctrl + V - paste
4.3 Word Edit Window
    Alt + R - record changes to the table and close the window
    Esc - don't record changes to the table and close the window
    Enter - save changes
    Alt + D - delete a word
    Alt + C - recover a word
    Alt + P - go to previous word
    Alt + N - go to next word
