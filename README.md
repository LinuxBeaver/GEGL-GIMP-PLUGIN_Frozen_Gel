# Frozen Gel - Ice cold text with dripping icicles

Create ice dripping frozen text effects with this GIMP plugin

![image](https://github.com/user-attachments/assets/6dc53fb1-31eb-429c-9c1b-8d1f692004f9)

![image](https://github.com/user-attachments/assets/f7297beb-d59f-4125-8b2f-7be9daaab757)

*Note this plugin is recommended for GIMP 3 only, and might not work on GIMP 2.10 unless you have GEGL 0.4.50+  , most builds of GIMP 2.10 ship with GEGL 0.4.48


## Directory to put Binaries (They do NOT go in the normal plugins folder)

**Windows**

 `C:\Users\(USERNAME)\AppData\Local\gegl-0.4\plug-ins`

 **Linux**

`~/.local/share/gegl-0.4/plug-ins`

 **Linux (Flatpak includes Chromebook)**

`~/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins`

Then Restart Gimp and go to Filters>Text Styling and look for "Frozen Gel" or just search for it with /

GIMP 2.10 users will only find this in GEGL Operation drop down list and it will only work on 2.10 if they are using GEGL 0.4.50+, all normal windows builds of GIMP 2.10 ship with GEGL 0.4.48

## Compiling and Installing

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```

### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:

```bash
meson setup --buildtype=release build
ninja -C build
```

## More previews of this based plugin

![image](https://github.com/user-attachments/assets/2ec6153d-1ee7-4d31-8902-6e42136fdacd)

![image](https://github.com/user-attachments/assets/d5c53af0-4c2f-45e7-b8cc-00d47c9b1218)



