# DSA-Lib

A small collection of **generic data structures in C**.

This is a side project I’m building while learning DSA in C — mainly to practice how real-world compilation, linking, static libraries, dynamic libraries, and build systems work.  
It’s like a personal **toolbox** for my experiments.

**Note:** The code may be buggy, not type-safe in all cases, and not fully production-ready.  
For robust, well-tested C data structure libraries, I recommend you to consider:
- [GLib](https://developer.gnome.org/glib/)
- [Collections-C](https://github.com/srdja/Collections-C)

If you want to tweak or try out this library anyway, read the **Build & Installation** guide below.

---

## Build & Installation

**Clone this repo:**

```bash
git clone https://github.com/AshishBirajdar2004/DSA-Lib.git
cd DSA-Lib
```

**Build the static library:**

```bash
make
```

**Install system-wide:**

```bash
sudo make install
```

This will:
- Copy header files to /usr/local/include/dsa-lib/
- Copy the static library to /usr/local/lib/

**Uninstall:**

```bash
sudo make uninstall
```

## Using the library in your programs

1. **Example usage in your own foo.c:**

```c
#include <dsa-lib/list.h>
```

2. **Compile and link:**

```bash
gcc foo.c -ldsa -o foo
```

## Alternative: use it locally (without installing)

If you don’t want to install system-wide, you can also:

1. Keep this repo inside your project:

```bash
git clone https://github.com/AshishBirajdar2004/DSA-Lib.git
cd DSA-Lib
make
```

2. In foo.c:

```c
#include "../DSA-Lib/include/list.h"
```

2. Compile and link with:

```bash
gcc foo.c ../DSA-Lib/libdsa.a -I../DSA-Lib/include -o foo
```

