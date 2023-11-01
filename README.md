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
- [ ] Add more blocks
- [ ] Spawn Blocks at random
- [ ] Attach colors to block classes
- [ ] Move Screen update to seperate header file
- [ ] Rotate Collision Checking
- [ ] Game Menu
- [ ] Alternate Bindings
- [ ] High Score Persistence
