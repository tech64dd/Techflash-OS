# ![Techflash OS Logo](/misc/logo_64x64_embed.png) Techflash OS

# What is this?

A fixed version of Techflash-OS without compilin' errors and whatnot.

Yup, (he) actually did it, (he) (kinda) made an operating system!  
In its current state, it doesn't really do much,  
it kinda just barfs some messages on to the screen and down the serial port.

# Installation

* Clone the repo

  * `git clone https://github.com/tech64dd/Techflash-OS.git`

* cd into the folder

  * `cd Techflash-OS`

* Install xorriso (command for Arch Linux)

  * `sudo pacman -S xorriso`

* Run make

  * `make`

* Done! &nbsp;You should now have a bootable ISO image in `bin/TFOS_ISO.iso`.


<details>
 
<summary>Old build guide (deprecated)</summary>
 
* Clone the repo

  * `git clone https://github.com/techflashYT/Techflash-OS.git`

* cd into the folder

  * `cd Techflash-OS`

* Run the interactive configuration script

  * `./configure`

* Run the `all` script

  * `./all.sh` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(This might take a while, so give it about a minute)

* Done! &nbsp;You should now have a bootable ISO image in `bin/TFOS_ISO.iso`.

 
</details>


# Booting

### Virtual Machine

Pretty easy, just set the ISO as the CD/DVD drive of the virtual machine and start it up!

### Real hardware

* ~~I ***HIGHLY*** recommend that you burn to a CD/DVD (or Re-Writeable USB) rather than to a regular USB. &nbsp;It would be a major waste of phyiscal discs since the CD/DVD can't be rewritten, and a USB wouldn't be permanently written.~~

* Some programs for this include: [Rufus (Burn to USB)](), &nbsp;[Ventoy (Boot ISO from USB)](), &nbsp;[Etcher (Burn to USB)](), &nbsp;or any generic ISO burner for burning to a CD/DVD.

# Bugs

None currently.  But since functionaly is extremely limited, it's kind of hard for there to be any bugs.
