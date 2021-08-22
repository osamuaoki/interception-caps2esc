# caps2esc

_Transforming the most useless key **ever** in the most useful one._
<sub>_For vi/Vim/NeoVim addicts at least_.</sub>

<a href="http://www.catonmat.net/blog/why-vim-uses-hjkl-as-arrow-keys/">
    <img src="http://www.catonmat.net/images/why-vim-uses-hjkl/lsi-adm3a-full-keyboard.jpg" alt="ADM-3A terminal">
</a>

## What is it?

- **Put what's useless in its place**
  <sub>_By moving the CAPSLOCK function to the far ESC location_</sub>
- **Make what's useful comfortably present, just below your Pinky**
  <sub>_By moving both ESC and CTRL functions to the CAPSLOCK location_</sub>

## Why?!

Because CAPSLOCK is just "right there" and making it CTRL when key-chording and
ESC when pressed alone is quite handy, specially in vi.

## Dependencies

- [Interception Tools][interception-tools]

## Building

```text
$ git clone https://gitlab.com/interception/linux/plugins/caps2esc.git
$ cd caps2esc
$ cmake -B build -DCMAKE_BUILD_TYPE=Release
$ cmake --build build
```

## Execution

```text
caps2esc - transforming the most useless key ever in the most useful one

usage: caps2esc [-h | [-m mode] [-t delay]]

options:
    -h        show this message and exit
    -t        delay used for key sequences (default: 20000 microseconds)
    -m mode   0: default
                 - caps as esc/ctrl
                 - esc as caps
              1: minimal
                 - caps as esc/ctrl
              2: useful on 60% layouts
                 - caps as esc/ctrl
                 - esc as grave accent
                 - grave accent as caps
```

`caps2esc` is an [_Interception Tools_][interception-tools] plugin. A suggested
`udevmon` job configuration (check the [_Interception Tools_
README][interception-tools] for alternatives) is:

```yaml
- JOB: intercept -g $DEVNODE | caps2esc | uinput -d $DEVNODE
  DEVICE:
    EVENTS:
      EV_KEY: [KEY_CAPSLOCK, KEY_ESC]
```

For more information about the [_Interception Tools_][interception-tools], check
the project's website.

## Mouse/Touchpad Support

After _Interception Tools_ 0.3.2, `caps2esc` can observe (or replace) mouse
events. An example configuration taken from my laptop:

```yaml
SHELL: [zsh, -c]
---
- CMD: mux -c caps2esc
- JOB: mux -i caps2esc | caps2esc | uinput -c /etc/interception/keyboard.yaml
- JOB: intercept -g $DEVNODE | mux -o caps2esc
  DEVICE:
    LINK: /dev/input/by-path/platform-i8042-serio-0-event-kbd
- JOB: intercept $DEVNODE | mux -o caps2esc
  DEVICE:
    LINK: /dev/input/by-path/platform-i8042-serio-4-event-mouse
```

For more information on the topic, check the [_Interception Tools_
README][interception-tools] about usage of the `mux` tool and hybrid virtual
device configurations.

## Installation

### Archlinux

It's available from [community](https://archlinux.org/packages/community/x86_64/interception-caps2esc/):

```text
$ pacman -S interception-caps2esc
```

### Void Linux

```text
$ xbps-install -S caps2esc
```

### Ubuntu ([independent package][ubuntu])

```text
sudo add-apt-repository ppa:deafmute/interception
sudo apt install interception-caps2esc
```

<sub>For debian and other derivatives you can download directly at https://launchpad.net/~deafmute/+archive/ubuntu/interception/+packages.</sub>

[ubuntu]: https://gitlab.com/interception/linux/tools/-/issues/38

## Caveats

As always, there's always a caveat:

- `intercept -g` will "grab" the detected devices for exclusive access.
- If you tweak your key repeat settings, check whether they get reset.
  Please check [this report][key-repeat-fix] about the resolution.

## History

I can't recall when I started using CAPSLOCK as both ESC and CTRL but it has
been quite some time already. It started when I was on OS X where it was quite
easy to achieve using the [Karabiner][], which already provides an option to
turn CTRL into ESC/CTRL (which can be coupled with OS X system settings that
turn CAPSLOCK into CTRL).

Moving on, permanently making Linux my home, I searched and tweaked a similar
solution based on [xmodmap][] and [xcape][]:

- <https://github.com/alexandre/caps2esc>

It's a simple solution but with many annoying drawbacks I couldn't stand in the
end:

- It resets any time a device change happens (bluetooth, usb, any) or the
  laptop lid is closed or when logging off and needs to be re-executed.
- It depends on [X][]. Doesn't work on TTY (bare terminal based machine,
  CTRL-ALT F2, etc).

Meanwhile on Windows land, I had a definitive solution based on my
[Interception library][interception] that always works perfectly, no hiccups.

It made me envy enough, so I ported the
[Windows Interception caps2esc][caps2esc-windows] sample to Linux based upon
the [_Interception Tools_][interception-tools].

## License

<a href="https://gitlab.com/interception/linux/plugins/caps2esc/blob/master/LICENSE.md">
    <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/0/0b/License_icon-mit-2.svg/120px-License_icon-mit-2.svg.png" alt="MIT">
</a>

Copyright © 2017 Francisco Lopes da Silva

[caps2esc-windows]: https://github.com/oblitum/Interception/blob/master/samples/caps2esc/caps2esc.cpp
[karabiner]: https://pqrs.org/osx/karabiner/
[xmodmap]: https://www.x.org/releases/X11R7.7/doc/man/man1/xmodmap.1.xhtml
[xcape]: https://github.com/alols/xcape
[x]: https://www.x.org
[interception]: https://github.com/oblitum/Interception
[interception-tools]: https://gitlab.com/interception/linux/tools
[key-repeat-fix]: https://github.com/oblitum/caps2esc/issues/1
