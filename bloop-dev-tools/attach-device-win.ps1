#!/usr/bin/env -S powershell.exe -ExecutionPolicy Bypass
usbipd wsl list
usbipd wsl attach -v 0483:374b 
