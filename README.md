<img src="cryo.svg" align="right" width="128" height="128">

# Wyvern
A tool to alter fan modes of MSI laptops and something more...  
Heavily inspired by [isw](https://github.com/YoyPa/isw) and [msi-ec](https://github.com/BeardOverflow/msi-ec).

![Wyvern UI](ui.png)

## Features
 - Fan profile chaging
 - Custom fan curve support
 - System performance mode selection
 - Battery charge threshold setting
 - Webcam lock
 - FN key swap

## Usage
`wyvern [subcommand] [options]`. Must be run as administrator.

### Fan
Commands related to the fan curves and modes.  
> Shift modes may limit fan speeds. If fan doesn't run at specific speed, try switching to turbo shift mode.   

`wyvern fan`: query current fan information.  
`wyvern fan --set {mode}`: set fan to specific mode.  
`wyvern fan --set custom -p [{temp},{speed_percent}]`: set custom fan curve. `-p` may be specified up to 6 times.  

### Shift modes
`wyvern shift`: get current shift mode.  
`wyvern shift {mode}`: switch to specific shift mode.  

### Webcam
`wyvern webcam`: get current webcam lock state.  
`wyvern webcam {enabled|disabled}`: set webcam lock state.  

### Battery
`wyvern battery`: get current battery charge limit.  
`wyvern battery {value}`: set battery charge limit.  

## Tested on
 - MSI Raider GE77 HX
 - MSI Raider GE78 HX
> EC may be flaky sometimes. If some setting is a bit off - just try applying it again
