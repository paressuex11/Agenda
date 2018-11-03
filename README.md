# Agenda

> 中山大学软件工程初级实训

## Resources

- Wiki: [https://wiki.vmatrix.org.cn](https://wiki.vmatrix.org.cn)

## Folder Structure

```
.
├── bin       # Output executables
├── build     # Object files to be used by GTest
├── data      # CSV files
├── gtest     # GTest files to test your code
├── include   # Header files
├── src       # Source files
├── Makefile
└── ...
```

## Build Setup

### Install GTest

```bash
# install source files
sudo apt-get install libgtest-dev

# install cmake
sudo apt-get install cmake

# compile the source files
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make

# copy or symlink *.a to your /usr/lib folder
sudo cp *.a /usr/lib
```

### Run GTest

```
. test.sh
```

### Compile

```bash
make clean
make
```
