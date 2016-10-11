# xattr_syscall_wrapper

Wrappers for the syscalls getxattr, setxattr, removexattr and listxattr.

## Motivation

There are `attr` and `getfattr`/`setfattr` which are standard parts of linux.
Unfortunately they seem to do a lot of checking for the caller, to the effect
that only `getfattr` as capable of reading `system.cifs_acl` and neither attr
nor `setfattr` where able to write it again. The same (probably) goes for other
system xattrs.

This repository provides plain wrappers around the syscalls and does zero extra
checks; the caller is responsible for encoding/decoding the data provided on
stdin/stdout and for choosing the correct namespace for the data.

Using these raw wrappers we can easily migrate windows acls between mounts (and posix
ACL as well probably and other cool things):

```
cp /mnt1/file /mnt2/file
getxattr /mnt1/file system.cifs_acl | setxattr /mnt2/file system.cifs_acl
```

## Building/Installing

```
[CXX=g++] make
sudo [prefix=/usr/local] make install
```

## Examples

Dumping the value of each xattr:

```
#! /bin/bash

set -e

listxattr -z "$1" | while IFS= read -r -d '' attr; do
  echo -n "$attr: "
  getxattr "$1" "$attr"
  echo
done
```

Copying all xattrs from one file to another:

```
#! /bin/bash

set -e

from="$1"
to="$2"

# Optionally purge all data on the dest file
listxattr -z "$to" | while IFS= read -r -d '' attr; do
  removexattr "$to" "$attr"
done

# Copy The Data
listxattr -z "$from" | while IFS= read -r -d '' attr; do
  getxattr "$from" "$attr" | setxattr "$to" "$attr"
done
```

## LICENSE

Written by (karo@cupdev.net) Karolin Varner, for Softwear, BV (http://nl.softwear.nl/contact): You can still buy me a Club Mate. Or a coffee.

Copyright © (c) 2016, Softwear, BV. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    Neither the name of the Karolin Varner, Softwear, BV nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Softwear, BV BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
