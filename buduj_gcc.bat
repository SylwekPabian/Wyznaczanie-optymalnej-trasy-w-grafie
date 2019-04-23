g++ -std=c++11 -D WIN32 -I ./fltk/include -L ./fltk/lib *.cpp -lfltk -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lshell32 -lcomctl32 -lole32 -loleaut32 -luuid -mwindows -o grafy.exe
