#define PRAGMA_ZERO

#include <acknex.h>
#include <litec.h>
#include <windows.h>
#include <stdio.h>

#include <default.c>

typedef BOOL bool;
#define NULL 0
#define null 0

int main() {

   wait(3);

   // rendering
   fps_max = 60;
   mip_flat = 2;
   d3d_antialias = 0;

   // window + system
   preload_mode = 7;
   mouse_pointer = 0;

   double resFac = 0.85; //%
   long resX = (double) sys_metrics(SM_CXSCREEN) * resFac;
   long resY = (double) sys_metrics(SM_CYSCREEN) * resFac;

   video_set(resX, resY, 0, 2);
   video_window(NULL, NULL, 0, "deadnex");

   wait(1);

   level_load(null);

   error("!");
}
