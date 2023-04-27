// Ben Roberts

struct ArdunioNeedsSetup: Decodable {
    /// Vault to request from
    let vault: VaultM
    /// Type of request for Arduino
    let setupResponse: String
}
