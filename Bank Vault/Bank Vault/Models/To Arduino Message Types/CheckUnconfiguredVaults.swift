// Ben Roberts

struct CheckUnconfiguredVaults: Encodable {
    /// Vault to request from
    let vault: VaultM
    /// ID of the user from this device
    let phoneID: Int = UserM.shared.userID ?? 0
    /// Type of request for Arduino
    let requestType: String = "request setup"
}
