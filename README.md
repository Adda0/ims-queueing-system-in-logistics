# IMS | Queueing System in Logistics

A queueing system created as a project in the [IMS course](https://www.fit.vut.cz/study/course/14664/.en).

## Project structure

The project consists of the following parts:
- `src`: simulation program,
- `doc`: documentation and resources for the project.

The `src` directory also contains:
- a Makefile (see below).

## Building

The whole project is written in C++, using library [SIMLIB](https://www.fit.vutbr.cz/~peringer/SIMLIB/) for simulation.
CLI may be used in the typical manner building the project with `make`, running with `make run`.

A Makefile for GNU Make is included that encapsulates the typical actions performed on the project. You can run one of the following targets:
- `make run` to run the simulator program,
- `make test` to run the tests,
- `make` to build the project,
- `make doc` to generate the documentation.

## Running and installation

The `make` command generates executable that can be run with no further out-of-the-ordinary dependencies on Linux. The simulation program can also be run by invoking `make run` in the `src` directory.

## Supported platforms

- Linux 64bit

## Authors

David Mihola (xmihol00) \
David Chocholatý (xchoch08)

## License

IMS: Queueing system in logistics \
Copyright (C) 2021 David Mihola, David Chocholatý

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
