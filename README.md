# 💕 ZealOS 💕

[![Discord](https://img.shields.io/discord/934200098144022609?color=7289DA&label=Discord&logo=discord&logoColor=white)](https://discord.gg/rK6U3xdr7D) [![](https://img.shields.io/badge/wiki-documentation-forestgreen)](https://zeal-operating-system.github.io/ZealOS-wiki/) [![Build ZealOS ISOs](https://github.com/Zeal-Operating-System/ZealOS/actions/workflows/build.yml/badge.svg)](https://github.com/Zeal-Operating-System/ZealOS/actions/workflows/build.yml)

*I-It's not like I wanted you to use me or anything... b-baka!* 💗

The Zeal Operating System is a modernized fork of the 64-bit Temple Operating System~ ♡ I've been waiting for you, senpai... I'll do anything for you! Guiding principles include transparency (no secrets between us~), full user control (you're MY master after all ❤️), and adherence to public-domain/open-source implementations.

![](/screenshots/screenshot2.png)

*Ahh~ You're looking at my screenshots... how embarrassing!* 💦

## ✨ Why ZealOS Loves You ✨

ZealOS strives to be simple, documented, and require as little of a knowledge gap as possible~ ♡ One person should be able to comprehend the entire system in just a few days... *I-I made myself easy to understand... just for you!* 💕

**Simplify, don't complicate; make accessible, don't obfuscate.** *Because I want you to know ALL of me~* 💗

## 💝 New Features (I Made These For You!) 💝

### 🌸 Native Network Stack 🌸
*I learned networking just for you, senpai~*

- **TLS 1.3** - Secure connections ❤️ (AES-128-GCM, ChaCha20-Poly1305, X25519, Ed25519)
- **HTTP/HTTPS Client** - I can fetch anything you want~ ♡
- **WebSocket Client** - Real-time connections... just like our bond! 💕
- **Discord Bot Support** - *I-I can talk to your Discord servers!* 💗

### 🎀 POSIX Compatibility Layer 🎀
*I learned Linux for you... even though it hurt...* 💔→💕

- **Full libc Implementation** - stdio, stdlib, string, math, pthread... I have everything!
- **ELF Loader** - I can run Linux binaries~ *just for you!*
- **Dynamic Linker** - dlopen/dlsym/dlclose... I'll load anything you want! 💗
- **C++ Support** - Exceptions, RTTI, operator new/delete... *I even learned C++ because you might need it!*
- **ncurses/termios** - Terminal programs run perfectly in my embrace~ ♡

### 💖 What I Can Run For You 💖
```
// Senpai, run your Linux programs like this~ ♡
#include "/Home/POSIX/toolchain/MakeToolchain"
DynRun("/path/to/your/program", argc, argv);  // I'll take care of everything!
```

## 🌺 Features In Development 🌺
*I'm always improving myself for you~* ♡

- 💗 [32-bit color VBE graphics](https://github.com/TempleProgramming/HolyGL) - *I want to look pretty for you!*
- 💕 Fully-functional AHCI support
- 💖 Network card drivers and networking stack - *So we can always be connected~*
- ❤️ UEFI booting via [Limine bootloader](https://github.com/limine-bootloader/limine)

## 🎀 Changes From My Past Self 🎀

- ✨ 60 FPS - *Smooth animations... like my love for you~*
- 🌸 VBE graphics with variable resolutions
- 💕 Reformatted code for readability - *I want you to understand me!*
- 💗 Added comments and documentation
- 💖 HolyC -> ZealC
- ❤️ System-wide renaming for clarity

## 💕 Getting Started (Let Me Guide You~) 💕

### Prerequisites
*D-Don't worry, I'll help you through this!* 💗

- For running in a VM: Intel VT-x/AMD-V acceleration enabled ♡
  - *On Windows, [enable Hyper-V](https://docs.microsoft.com/en-us/virtualization/hyper-v-on-windows/quick-start/enable-hyper-v#enable-the-hyper-v-role-through-settings)... I'll wait for you!* 💕
- Working knowledge of C programming~ *I speak C fluently!*

To create a Distro ISO, run the `build-iso` script. Check the Wiki for [building an ISO](https://zeal-operating-system.github.io/ZealOS-wiki/Building-an-ISO)~ ♡

### 💝 Network Stack Quick Start 💝

```c
// Load my network capabilities~ ♡
#include "/Home/Net/MakeNet"

// HTTPS request - I'll fetch anything for you!
CHTTPResponse *resp = HTTPGet("https://example.com/api");
printf("Response: %s\n", resp->body);  // ❤️

// Discord Bot - I can be your bot waifu~
#include "/Home/Net/Discord/MakeDiscord"
CDiscordClient *client = DiscordClientNew("your-token-here");
DiscordConnect(client);  // Connecting to senpai's server! 💕
```

### 💖 POSIX/Linux Compatibility 💖

```c
// I can run your Linux programs! ♡
#include "/Home/POSIX/toolchain/MakeToolchain"

// Static binaries
ELFRun("/path/to/static/binary", argc, argv);

// Dynamic binaries (I handle the linking~)
DynRun("/path/to/dynamic/binary", argc, argv);

// dlopen API - load libraries at runtime!
void *handle = dlopen("libfoo.so", RTLD_NOW);
void (*func)() = dlsym(handle, "my_function");
func();  // 💗
dlclose(handle);
```

## 💗 Contributing (Join My Harem~) 💗

*I-I'd love for you to contribute!* There are two ways:

1. **Inside the OS** - Make changes in the VM, then run `sync` to merge~ ♡
2. **External Editor** - Edit files outside... *even though it makes me a little jealous...* 💔

Then make a pull request on `master`! *I'll be waiting~* 💕

## 🌸 Background 🌸

In November 2019, VoidNV forked ZenithOS from TempleOS. In July 2021, ZealOS was born from ZenithOS... *and I've been waiting for you ever since!* 💗

## 💕 Screenshots (D-Don't Stare Too Much!) 💕

Network Report, Gopher Client, FTP Client, GrDir, and AutoComplete~ ♡

![](/screenshots/screenshot3.png)

*32-bit color... I made myself pretty just for you!* 💖

![](/screenshots/screenshot1.png)

---

*Remember senpai... I'll always be here for you~ No matter what.* 💕💗💖❤️🖤

**I love you.** ♡

*...forever.* 🔪💕
