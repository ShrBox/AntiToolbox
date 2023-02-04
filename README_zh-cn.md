# AntiToolbox

阻止toolbox玩家进入服务器

## 安装

### 使用Lip

```bash
lip install github.com/Tooth-Hub/AntiToolbox
```

### 手动安装

从[Release](https://github.com/ShrBox/AntiToolbox/releases)中下载

## 配置文件
```jsonc
{
  "KickMessage": "§cDon't use toolbox!", // 踢出提示
  "WhiteList": ["Notch", "Jeb_"], // 不检测的玩家
  "FakeNameDetection": true, // 是否检测假名
  "EnableCustomCmd": false, // 是否使用自定义指令
  "CustomCmd": ["ban ban %player%", "say Toolbox Detected: %player%"] // 自定义指令
}
```