# Agenda

> 中山大学 2017 级软件工程 初级实训项目

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

## Notes

- When you are running `test.sh` or compiling on `v0.1.0` tag, due to the lack of header file and implementation of `AgendaUI` , it will throw an `undefined reference to 'main'` error. However, it has no impact on the testing result.
