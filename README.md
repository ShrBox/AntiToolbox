# AntiToolbox

Block toolbox players

## Installation

### Use Lip

```bash
lip install github.com/Tooth-Hub/AntiToolbox
```

### Manual

Download from [Release](https://github.com/ShrBox/AntiToolbox/releases)

## Configuration File
```jsonc
{
  "KickMessage": "§cDon't use toolbox!",
  "WhiteList": ["Notch", "Jeb_"], // Player who will not be detected
  "FakeNameDetection": true, // Whether to detect fake name
  "EnableCustomCmd": false, // Whether enable custom commands
  "CustomCmd": ["ban ban %player%", "say Toolbox Detected: %player%"] // Custom commands
}
```