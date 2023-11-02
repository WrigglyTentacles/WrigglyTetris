# WrigglyTetris
## Dependencies
### Build Toolchain
#### Ubuntu/Debian Example
```sh
sudo apt install build-essential
```
### Meson and Ninja
#### Python Pip install
```python
pip3 install meson ninja
```
### Alternatively you can use your Distro
### Ubuntu/Debian
```sh
sudo apt-get install meson ninja-build
```
#### More about meson can be found here.
[Install Meson](https://mesonbuild.com/Getting-meson.html)

### libncurses5-dev
#### Ubuntu/Debian
```sh
sudo apt-get install libncurses5-dev
```
## Setup Script
```sh
./setup.sh
```
## Build and Run Script
```sh
./bar.sh
```
### TODO
- [x] Block Rotation
- [x] Clear Line and Score Points
- [x] Collision with Near blocks and Game board
- [x] Add more blocks
- [x] Spawn Blocks at random
- [x] Drop Orphaned blocks once line has been completed
- [x] Block Preview
- [x] Block Preview Block exchange
- [x] Attach colors to block classes
- [x] Set Block quick drop
- [x] Block Preview Block exchange
- [ ] Block Shadow where it will collide on quick drop
- [ ] Move Screen update to seperate header file
- [ ] Rotate Collision Checking
- [ ] Game Menu
- [ ] Alternate Bindings
- [ ] High Score Persistence
