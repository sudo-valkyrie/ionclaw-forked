---
name: qrcode
description: Generate QR code PNG images from text, URLs, WiFi configs, vCard contacts, or any data. Use when the user asks to create a QR code, "二维码", "扫码", "QR", or wants to encode text into a scannable image.
platform: [linux, macos, windows]
---

# QR Code Generator

Use the built-in `qrcode` tool to generate QR code images.

## Basic usage

```
qrcode(data="https://example.com")
```

The tool saves a PNG to the workspace and returns its path.

## Parameters

| Parameter     | Required | Default | Description |
|--------------|----------|---------|-------------|
| `data`       | yes      | —       | Content to encode (URL, text, WiFi, vCard) |
| `filename`   | no       | auto    | Output PNG filename |
| `error_level`| no       | `M`     | L=7%  M=15%  Q=25%  H=30% |
| `box_size`   | no       | 10      | Module size in pixels (1–100) |
| `border`     | no       | 4       | White border in modules (0–100) |

## Common scenarios

### URL / link
```
qrcode(data="https://github.com/ionclaw-org/ionclaw")
```

### WiFi (scan to connect)
```
qrcode(data="WIFI:T:WPA;S:MyNetwork;P:password123;;")
```

### vCard / contact
```
qrcode(data="BEGIN:VCARD\nFN:张三\nTEL:13800138000\nEND:VCARD")
```

### Plain text
```
qrcode(data="Hello IonClaw", filename="hello.png", box_size=20)
```

## Tips

- Use `error_level=H` when the QR code will have a logo overlaid (via `image_ops`)
- Use larger `box_size` for big posters, smaller for thumbnails
- WiFi format: `WIFI:T:WPA;S:network_name;P:password;;`
- vCard format starts with `BEGIN:VCARD` and ends with `END:VCARD`
