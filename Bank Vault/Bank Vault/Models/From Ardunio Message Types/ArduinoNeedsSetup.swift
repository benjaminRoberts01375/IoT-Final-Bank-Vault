// Ben Roberts

struct ArdunioNeedsSetup: Decodable {
    /// Vault to request from
    let vault: VaultM
    /// ID of the user from this device
    let phoneID: Int
    /// Type of request for Arduino
    let setupResponse: String
}
