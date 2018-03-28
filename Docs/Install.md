# Link-TLM Installation #

Welcome to `Link-TLM`! If you're an experienced user/developer of `Link-TLM`, you can skip ahead the the [Dependencies](#dependencies) section.
If not, greetings new BPP tracker and/or developer! This here guide will help you get `Link-TLM` built & running for the first time.

## Getting Started ##

First of all: `Link-TLM` is a Linux-only program. It will not build on Windows or Mac. If you are not on a Linux computer, stop reading and get access to one!
There are two Linux laptops in the balloon lab that have been set up specifically for tracking purposes. A VM (Virtual Machine) won't work, as `Link-TLM` needs
USB port access, and typically setting that up on a VM is a massive pain. (If you manage to get it, please update this doc with instructions!)

If you are wholly unfamiliar with Linux, I suggest you learn at least the basics of using the Linux command line - it will make this process much easier.
In particular, the `cd`, `ls`, `mv`, `cp`, and `rm` commands are probably the most important. Also learn about `git`; `apt-get` would be good, too. (`apt-get`
is not universal to Linux, but the Lab laptops run a version of Linux, called Debian, which uses it.)

If you are familiar with Linux, great!

`Link-TLM` is written in C++11, with support scripts written in Python. I specify C++11 because it was a huge update to the language, and some older compilers
don't support it yet - detail is below. C++ was chosen because, at the time, I viewed it as the right tool for the job, and I actually quite like C++. In
hindsight, Python might have been okay, but I do think I had a better time of it using C++. Larger Python projects, in my experience, tend to get both slow
and hacked together; and the "Python Way" sometimes gets _in the way_ of what you want to do. However, for the support scripts, Python is the perfect choice.
GUI programming is one area where C++ is terribly weak, and Python is quite good, so the trade is in Python's favor there. This brief ramble hopefully provides
some insight into my choice of language for those who care about such things!

## Dependencies ##
