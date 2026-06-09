# my dotfiles

republished my own collection of dotfiles for suckless env in one repo. \
In other words, this is dwm thats sucks more.
## Mini-review

#### feh required! (for static wallpaper)
dmenu at bottom\
mod+o opens playerctl dmenu\
my own wallpaper\
catppuccin themed!!\
floating mode priority\
only 5 tags\
WAIT WHAT??? LIVE WALLPAPERS!!! (my own)\
Widget system based on nuklear rendering in same ogl context 

Feel free to implement your own widgets!! (see nktest() in dwm/dwm.c for example. ) 

You can view a list of wallpapers in dwm/glsl/ and choose in config.h
### patches:
1. awesome taskbar
2. fullscreen fix
3. window focus fix
4. systray
5. powerline-dmenu
6. refreshrate fix

#### thanks to suckless patches!

# Depedencies
0. pkgconf and python ofc for waf buildsystem
1. dwm, dmenu, st, and slstatus depedencies oviviously (just `apt build-dep` them!)
2. libgl and libglx

# Building:
1. clone the repo.
2. `./waf configure build`
3. `sudo ./waf install`
4. Thats it!
