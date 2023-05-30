We often want to clone our RPI setup , in case something bad happens to our Pi, or in case our software configurations get so messed up we can't undo it.

Cloning can take a long time, but this method below is the fastest one I could find:

git clone https://github.com/billw2/rpi-clone.git
cd rpi-clone
sudo cp rpi-clone rpi-clone-setup /usr/local/sbin
Now to do a clone:

sudo rpi-clone -s raspberrypi sda

NOTE: the raspberrypi in the above command is your pi's hostname. If you've used a different hostname, change it in the command above

NOTE: Come up with a godo solutions for how you're goign to STORE and ORGANIZE your tiny little SD cards...
