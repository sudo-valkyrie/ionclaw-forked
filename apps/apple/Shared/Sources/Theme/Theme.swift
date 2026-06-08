import SwiftUI

// brand palette, mirrored from the flutter runner
enum Theme {
    static let primary = Color(hex: 0x0A8DCF)
    static let success = Color(hex: 0x2E7D32)
    static let running = Color(hex: 0x4CAF50)
    static let danger = Color(hex: 0xC62828)

    static let header = Color(hex: 0x1A1A2E)
    static let screen = Color(hex: 0xF8F9FA)
    static let card = Color(hex: 0xFFFFFF)
    static let cardBorder = Color(hex: 0xE5E7EB)
    static let muted = Color(hex: 0x9E9E9E)
    // fixed text colors so the light card stays readable regardless of system dark mode
    static let label = Color(hex: 0x1C1C1E)
    static let secondaryLabel = Color(hex: 0x6B7280)
}

extension Color {
    init(hex: UInt32) {
        let red = Double((hex >> 16) & 0xFF) / 255
        let green = Double((hex >> 8) & 0xFF) / 255
        let blue = Double(hex & 0xFF) / 255

        self.init(red: red, green: green, blue: blue)
    }
}
