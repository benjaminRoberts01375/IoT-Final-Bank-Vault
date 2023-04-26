// Ben Roberts

struct ArduinoSetupRequest: Codable {
    let vault: VaultM
    let phoneID: Int
    let confirmSetup: Bool
}
