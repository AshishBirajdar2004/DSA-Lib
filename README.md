# DSA-Lib

A small collection of **generic data structures in C**.

This is a side project I’m building while learning DSA in C — mainly to learn how real-world compilation, linking, static libraries, dynamic libraries, and build systems work.  
It’s like a personal **toolbox**.

**Note:** The code may be buggy, not type-safe in all cases, and not fully production-ready.  
For robust, well-tested C data structure libraries, I recommend you consider::
- [GLib](https://developer.gnome.org/glib/)
- [Collections-C](https://github.com/srdja/Collections-C)

If you want to tweak or try out this library anyway, read the **Build & Installation** guide below.

---

## Build & Installation

**Build & Install:**

```bash
git clone https://github.com/AshishBirajdar2004/DSA-Lib.git
cd DSA-Lib
make
sudo make install
```

This will:
- Build the static library (libdsa.a)
- Copy header files to /usr/local/include/dsa-lib/
- Copy the static library to /usr/local/lib/

**Note:** sudo is required to install the library system-wide under /usr/local.

**Running tests:**

This will compile all test programs into the .build/ directory. To run an test, execute its corresponding binary. For example:

```bash
make tests
.build/arraylist-test
```

**Uninstall:**

```bash
sudo make uninstall
```

---

## Using the library in your programs

1. Example usage in your own foo.c:

```c
#include <dsa-lib/arraylist.h>
```

2. Compile and link:

```bash
gcc foo.c -ldsa -o foo
```

---

## Alternative: use it locally (without installing)

If you don’t want to install system-wide, you can also just keep the **include** directory and the **static library (libdsa.a)** in your project folder:

1. Example usage in your own foo.c:

```c
#include "include/arraylist.h"
```

2. Compile and link:

```bash
gcc foo.c libdsa.a -Iinclude -o foo
```
