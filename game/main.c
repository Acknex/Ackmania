#define PRAGMA_ZERO

#include <acknex.h>
#include <litec.h>
#include <windows.h>
#include <stdio.h>

#include <default.c>

#define PRAGMA_PATH "scripts";
#define PRAGMA_PATH "scripts\\main";

#include "sysdefines.h"
#include "sysinit.h"

int main() {

   wait(3);

   doSysInit();

   wait(1);
   level_load(null);

   error("!");
}
