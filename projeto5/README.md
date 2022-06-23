
# Project 5 addind our on Syscall

For this project we added two new system call in the linux kernel. Te changes can be found in the changes.patch in this repo.

to apply the changes in your linux kernel code use the command:
```
git apply --reject --whitespace=fix changes.patch
```
in the kernel root directory.