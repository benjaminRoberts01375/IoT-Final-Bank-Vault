// Ben Roberts

struct ArduinoIsSetup: Decodable {
    /// Vault to request from
    let vault: VaultM
    /// ID of the mobile device
    let phoneID: String
    /// Tracker for if the setup was complete
    let confirmSetup: Bool
}
