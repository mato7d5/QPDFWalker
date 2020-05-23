# QPDFWalker
QPDFWalker is tool for browsing hierarchy of low level objects in a PDF file. It was
inspired by Enfocus browser.
Currently the QPDFWalker runs on GNU Linux and MS Windows. It should not be a problem
to build it for other platforms (FreeBSD, NetBSD, MacOS,...).   

### Based on
QPDFWalker uses:
* [Qt 5](https://www.qt.io/)
* [PoDoFo](http://podofo.sourceforge.net/)

### Build
#### GNU Linux
Install all required dependencies (Qt 5 and PoDoFo) by using your package manager of your Linux
distribution. You can build it by Qt Creator (open qpdfwalker.pro file) or by shell:
```
qmake-qt5 qpdfwalker.pro
make
```
#### MS Windows
The best way is usage of Cygwin environment. Don't forget to install PoDoFo and Qt 5 packages for Cygwin.
Open bash and enter:
```
qmake-qt5 qpdfwalker.pro
make
```
Launching qpdfwalker.exe:
```
XWin.exe --multiwindow
export DISPLAY=:0.0
qpdfwalker.exe
```

### Screenshots
![Main window](readme_images/main_window.png?raw=true "Main window")

![Walker window](readme_images/walk_window.png?raw=true "Walker window")

![Stream data window](readme_images/stream_data.png?raw=true "Stream data window")

### License
GNU GPL v3.
