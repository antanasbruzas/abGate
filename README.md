# abGate

	=================
	abGate LV2 Plugin
	=================

abGate is LV2 noise gate plugin for Linux.

Newest version of the plugin can be downloaded from https://abgate.sourceforge.io or from the github page: https://github.com/antanasbruzas/abGate/


Installation
------------
abGate plugin will be installed into /usr/lib/lv2 by default
If you want to change the installation path, edit Makefile and plugin_configuration.h files

Before installing the plugin from source check if you have all of the required libraries:

  - gcc compiler
  - pkg-config
  - gtkmm >= 2.4
  - lv2core
  - qmake

To install the plugin, go to abGate folder in the terminal and type:
 
  make

If make finish successfully type the following as root

  make install

That's it! Have fun using this plugin :)
