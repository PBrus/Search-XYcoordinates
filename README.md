# Search-XYcoordinates [![GitHub release](http://www.astro.uni.wroc.pl/ludzie/brus/img/github/ver20170120.svg "download")](https://github.com/PBrus/Search-XYcoordinates) ![Written in C](http://www.astro.uni.wroc.pl/ludzie/brus/img/github/C.svg "language")

Allows to search for common stars using XY coordinates. It was written in pure C.

## Installation

Download the files from the repository to your working directory. Additionally you need to have installed the following software:

 * *gcc*
 * *make*

Run the terminal window, go to your working directory and then type:
```bash
$ make
```
This call allows to compile the program. If you want to clean indirect object files created during the last call, please type:
```bash
$ make clean
```

## Usage

To use the program properly you need to prepare input lists. At the beginning call the program from the terminal with the `--help` option:
```bash
$ ./sdb_xy --help
```
This will give you a description about all options. If you need to see the program in action immediately, please type:
```bash
$ ./sdb_xy list1 list2 -h 3
```
More advanced call involves the rest of options and their combinations. One of them can be:
```bash
$ ./sdb_xy list1 list2 -h 3 -m 1 -r 3.1 -o 1.7 -2.9 -s 6
```
All outputs will be printed to the standard output. Usage of the `-g` option saves some information to `sdb_xy.reg` file.

I encourage to visit my website to see more detailed description of this program. The current link can be found on my [GitHub profile](https://github.com/PBrus).

## License

**Search-XYcoordinates** is licensed under the [MIT license](http://opensource.org/licenses/MIT).
