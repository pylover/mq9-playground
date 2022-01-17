PORT = /dev/ttyACM0
BAUDRATE = 115200
PACKAGER = arduino
SKETCH = mqpg
ARCH = avr
BOARD = uno
FQBN = $(PACKAGER):$(ARCH):$(BOARD)

.PHONY: env
env:
	arduino-cli core update-index
	arduino-cli core install $(PACKAGER):$(ARCH)
	arduino-cli lib install --git-url https://github.com/pylover/nash.git

.PHONY: sketch
sketch:
	arduino-cli compile --fqbn $(FQBN) $(SKETCH)

.PHONY: upload
upload: sketch
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) $(SKETCH)

.PHONY: clean
clean:
	arduino-cli cache clean

.PHONY: screen
screen:
	screen $(PORT) $(BAUDRATE)
