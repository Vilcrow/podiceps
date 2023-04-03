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

